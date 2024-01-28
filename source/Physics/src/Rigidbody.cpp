#include "Arithmetic.h"
#include "Rigidbody.h"
#include "Component.h"
#include "Entity.h"
#include "ICollider.h"
#include "Debug/Log.h"
#include "Utility/ServiceLocator.h"
#include "Utility/Timer.h"

using namespace LibMath;
using namespace LibGL::Utility;

namespace LibGL::Physics
{
    Rigidbody::Rigidbody(Entity& owner)
        : Component(owner)
    {
    }

    void Rigidbody::update()
    {
        Component::update();

        simulate();
    }

    void Rigidbody::addForce(const Vector3& force, const EForceMode forceMode)
    {
        if (!isActive() || m_isKinematic)
            return;

        switch (forceMode)
        {
        case EForceMode::FORCE:
            m_velocity += force * LGL_SERVICE(Timer).getDeltaTime() / m_mass;
            break;
        case EForceMode::ACCELERATION:
            m_velocity += force * LGL_SERVICE(Timer).getDeltaTime();
            break;
        case EForceMode::IMPULSE:
            m_velocity += force / m_mass;
            break;
        case EForceMode::VELOCITY_CHANGE:
            m_velocity += force;
            break;
        default:
            const std::string msg = formatString("Invalid force mode: %u\n", forceMode);
            DEBUG_LOG(msg.c_str());
            throw std::out_of_range(msg);
        }
    }

    void Rigidbody::sleep()
    {
        m_isSleeping = true;
    }

    void Rigidbody::wakeUp()
    {
        m_isSleeping = false;
    }

    bool Rigidbody::isSleeping() const
    {
        return m_isSleeping;
    }

    Vector3 Rigidbody::getDraggedVelocity() const
    {
        const float deltaTime = LGL_SERVICE(Timer).getDeltaTime();
        return deltaTime > 0.f ? m_velocity * clamp(1.f - m_drag * deltaTime, 0.f, 1.f) : Vector3::zero();
    }

    void Rigidbody::simulate()
    {
        if (!isActive())
            return;

        if (m_isKinematic)
        {
            move();
            return;
        }

        if (m_useGravity)
            addForce(g_gravity, EForceMode::ACCELERATION);

        if (isSleeping())
        {
            if (m_velocity.magnitudeSquared() >= m_sleepThreshold)
                wakeUp();

            return;
        }

        if (floatEquals(getDraggedVelocity().magnitudeSquared(), 0.f))
            return;

        move();
    }

    void Rigidbody::move()
    {
        if (!isActive() || isSleeping())
            return;

        const float deltaTime = LGL_SERVICE(Timer).getDeltaTime();

        if (m_isKinematic)
            getOwner().translate(m_velocity * deltaTime);

        const auto ownerColliders = getOwner().getComponents<ICollider>();

        if (ownerColliders.empty())
        {
            getOwner().translate(getDraggedVelocity() * deltaTime);
            return;
        }

        const auto worldColliders = ICollider::getColliders();

        int stepsCount;

        switch (m_collisionDetectionMode)
        {
        case ECollisionDetectionMode::DISCRETE:
            stepsCount = 1;
            break;
        case ECollisionDetectionMode::CONTINUOUS:
            stepsCount = g_continuousCollisionSteps;
            break;
        case ECollisionDetectionMode::NONE:
        default:
            getOwner().translate(getDraggedVelocity() * deltaTime);
            return;
        }

        std::unordered_map<ComponentId, std::vector<ComponentId>> checkedCollidersMap;

        for (int i = 0; i < stepsCount; i++)
        {
            const Vector3 velocity = getDraggedVelocity();

            for (const auto& entityCollider : ownerColliders)
            {
                if (!entityCollider->isActive())
                    continue;

                auto& checkedColliders = checkedCollidersMap[entityCollider->getId()];

                for (const auto& worldCollider : worldColliders)
                {
                    if (worldCollider == nullptr || !worldCollider->isActive() ||
                        &worldCollider->getOwner() == &getOwner() || *entityCollider == *worldCollider ||
                        std::ranges::find(checkedColliders, worldCollider->getId()) != checkedColliders.end() ||
                        !entityCollider->check(*worldCollider))
                        continue;

                    const Vector3 normal = getBoundsNormal(*entityCollider, *worldCollider);

                    const Vector3 normalMask
                    {
                        LibMath::abs(normal.m_x) > 0.f ? sign(normal.m_x) : 0.f,
                        LibMath::abs(normal.m_y) > 0.f ? sign(normal.m_y) : 0.f,
                        LibMath::abs(normal.m_z) > 0.f ? sign(normal.m_z) : 0.f
                    };

                    const Vector3 frictionMask
                    {
                        floatEquals(normalMask.m_x, 0.f) ? sign(normalMask.m_x) : 0.f,
                        floatEquals(normalMask.m_y, 0.f) ? sign(normalMask.m_y) : 0.f,
                        floatEquals(normalMask.m_z, 0.f) ? sign(normalMask.m_z) : 0.f
                    };

                    Rigidbody* otherRigidbody = worldCollider->getOwner().getComponent<Rigidbody>();

                    // There is a collision, apply opposite forces
                    if (otherRigidbody != nullptr && otherRigidbody != this && otherRigidbody->isActive())
                    {
                        const Vector3 otherVelocity = otherRigidbody->getDraggedVelocity();

                        if (!otherRigidbody->m_isKinematic)
                        {
                            if (normal.dot(otherVelocity) >= 0.f)
                                otherRigidbody->addForce((otherVelocity * normalMask).magnitude() * -normal,
                                    EForceMode::VELOCITY_CHANGE);

                            if (normal.dot(-velocity) >= 0.f)
                                otherRigidbody->addForce((velocity * m_mass * normalMask).magnitude() * -normal,
                                    EForceMode::IMPULSE);
                        }

                        if (normal.dot(-velocity) >= 0.f)
                            addForce((velocity * normalMask).magnitude() * normal, EForceMode::VELOCITY_CHANGE);

                        if (normal.dot(otherVelocity) >= 0.f)
                            addForce((otherVelocity * otherRigidbody->m_mass * normalMask).magnitude() * normal,
                                EForceMode::IMPULSE);
                    }
                    else if (normal.dot(-velocity) >= 0.f)
                    {
                        addForce((velocity * normalMask).magnitude() * normal, EForceMode::VELOCITY_CHANGE);
                    }

                    addForce(-velocity * frictionMask * g_friction * g_gravity.magnitude(), EForceMode::ACCELERATION);

                    checkedColliders.push_back(worldCollider->getId());
                }
            }

            const Vector3 step = getDraggedVelocity() * deltaTime / static_cast<float>(stepsCount);
            getOwner().translate(step);
        }

        if (getDraggedVelocity().magnitudeSquared() < m_sleepThreshold * m_sleepThreshold)
            sleep();
    }

    Vector3 Rigidbody::getBoundsNormal(const ICollider& entityCollider, const ICollider& worldCollider)
    {
        const auto [center, size, _] = worldCollider.getBounds();
        const auto minCorner = center - size;
        const auto maxCorner = center + size;

        const auto entityCenter = entityCollider.getBounds().m_center;

        const Vector3 snappedX
        {
            snap(entityCenter.m_x, minCorner.m_x, maxCorner.m_x),
            entityCenter.m_y,
            entityCenter.m_z
        };

        const Vector3 snappedY
        {
            entityCenter.m_x,
            snap(entityCenter.m_y, minCorner.m_y, maxCorner.m_y),
            entityCenter.m_z
        };

        const Vector3 snappedZ
        {
            entityCenter.m_x,
            entityCenter.m_y,
            snap(entityCenter.m_z, minCorner.m_z, maxCorner.m_z)
        };

        const float minDist = min(entityCenter.distanceSquaredFrom(snappedX),
            min(entityCenter.distanceSquaredFrom(snappedY),
                entityCenter.distanceSquaredFrom(snappedZ)));

        if (floatEquals(minDist, entityCenter.distanceSquaredFrom(snappedX)))
            return (Vector3::right() * sign(snappedX.m_x - center.m_x)).normalized();

        if (floatEquals(minDist, entityCenter.distanceSquaredFrom(snappedY)))
            return (Vector3::up() * sign(snappedY.m_y - center.m_y)).normalized();

        if (floatEquals(minDist, entityCenter.distanceSquaredFrom(snappedZ)))
            return (Vector3::front() * sign(snappedZ.m_z - center.m_z)).normalized();

        return Vector3::zero();
    }
}

#include "BoxCollider.h"

#include "Arithmetic.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Debug/Log.h"
#include "Vector/Vector3.h"

using namespace LibMath;

namespace LibGL::Physics
{
    BoxCollider::BoxCollider(Entity& owner, const Vector3& center, const Vector3& size)
        : ICollider(owner, calculateBounds(center, size)), m_center(center), m_size(size)
    {
    }

    bool BoxCollider::check(const Vector3& point) const
    {
        const auto [center, size, _] = getBounds();

        const Vector3 min = center - size / 2.f;
        const Vector3 max = center + size / 2.f;

        return min.m_x <= point.m_x && max.m_x >= point.m_x &&
            min.m_y <= point.m_y && max.m_y >= point.m_y &&
            min.m_z <= point.m_z && max.m_z >= point.m_z;
    }

    bool BoxCollider::check(const Ray& ray, float& distanceSqr) const
    {
        // Check the bounding spheres first to avoid unnecessary computation
        if (!ICollider::check(ray, distanceSqr))
            return false;

        const auto [center, size, radius] = getBounds();

        const Vector3 boxMin = center - size / 2.f;
        const Vector3 boxMax = center + size / 2.f;

        const Vector3 dirInverse = Vector3::one() / ray.m_direction.normalized();

        // point = rayOrigin + t * rayDir <=> t = (point - rayOrigin) / rayDir
        const Vector3 distMinVec = (boxMin - ray.m_origin) * dirInverse;
        const Vector3 distMaxVec = (boxMax - ray.m_origin) * dirInverse;

        const float distMin = max(
            max(
                min(distMinVec.m_x, distMaxVec.m_x),
                min(distMinVec.m_y, distMaxVec.m_y)),
            min(distMinVec.m_z, distMaxVec.m_z)
        );

        const float distMax = min(
            min(
                max(distMinVec.m_x, distMaxVec.m_x),
                max(distMinVec.m_y, distMaxVec.m_y)),
            max(distMinVec.m_z, distMaxVec.m_z)
        );

        if (distMax < 0 || distMin > distMax)
        {
            distanceSqr = INFINITY;
            return false;
        }

        distanceSqr = distMin < 0.f ? distMax * distMax : distMin * distMin;
        return true;
    }

    bool BoxCollider::check(const ICollider& other) const
    {
        const auto otherTypeHash = typeid(other).hash_code();

        if (otherTypeHash == typeid(BoxCollider).hash_code())
            return checkBox(dynamic_cast<const BoxCollider&>(other));

        if (otherTypeHash == typeid(SphereCollider).hash_code())
            return checkSphere(dynamic_cast<const SphereCollider&>(other));

        if (otherTypeHash == typeid(CapsuleCollider).hash_code())
            return checkCapsule(dynamic_cast<const CapsuleCollider&>(other));

        DEBUG_LOG("WARNING: collisions between 'BoxCollider' and '%s' are not supported.\n", typeid(other).name());
        return false;
    }

    bool BoxCollider::checkBox(const BoxCollider& other) const
    {
        const auto [center, size, radius] = getBounds();
        const auto [otherCenter, otherSize, otherRadius] = other.getBounds();

        const Vector3 min = center - size / 2.f;
        const Vector3 max = center + size / 2.f;
        const Vector3 otherMin = otherCenter - otherSize / 2.f;
        const Vector3 otherMax = otherCenter + otherSize / 2.f;

        return min.m_x <= otherMax.m_x && max.m_x >= otherMin.m_x &&
            min.m_y <= otherMax.m_y && max.m_y >= otherMin.m_y &&
            min.m_z <= otherMax.m_z && max.m_z >= otherMin.m_z;
    }

    bool BoxCollider::checkSphere(const SphereCollider& other) const
    {
        // Check the bounding spheres first to avoid unnecessary computation
        if (!ICollider::check(other))
            return false;

        const auto sphereCenter = other.getBounds().m_center;

        // If the closest point to the sphere's center is in the sphere, the box and sphere collide
        return other.check(getClosestPoint(sphereCenter));
    }

    bool BoxCollider::checkCapsule(const CapsuleCollider& other) const
    {
        // Check the bounding spheres first to avoid unnecessary computation
        if (!ICollider::check(other))
            return false;

        // Compute the closest point on the box to the capsule
        const Vector3 halfSize = getBounds().m_boxSize * 0.5f;
        const Vector3 boxCenter = getBounds().m_center;

        const Vector3 capsuleCenter = other.getBounds().m_center;
        const float   capsuleRadius = other.getRadius();
        const float   capsuleHeight = other.getHeight();

        // Check if the capsule is inside of the box
        const Vector3 closestOnCapsule = other.getClosestPoint(boxCenter);
        const Vector3 closestOnBox = getClosestPoint(closestOnCapsule);

        if (check(closestOnCapsule) || other.check(closestOnBox))
            return true;

        Vector3 capsuleToBox = boxCenter - capsuleCenter;
        Vector3 closestPoint;

        // Check intersection on each axis
        for (int i = 0; i < 3; i++)
        {
            // First check if the capsule is above or below the box
            const float boxMin = boxCenter[i] - halfSize[i];
            const float boxMax = boxCenter[i] + halfSize[i];
            const float capsuleMin = capsuleCenter[i] - capsuleHeight * 0.5f;
            const float capsuleMax = capsuleCenter[i] + capsuleHeight * 0.5f;

            // Capsule is completely above or below the box, so there's no collision
            if (capsuleMin > boxMax || capsuleMax < boxMin)
                return false;

            // Check if the capsule is intersecting the box in the x-z plane
            capsuleToBox.m_y = 0.f;
            capsuleToBox = capsuleToBox.normalized();

            Vector3 boxExtents = halfSize - Vector3(capsuleRadius, 0.f, capsuleRadius);
            boxExtents = max(boxExtents, Vector3::zero());

            closestPoint = boxCenter + capsuleToBox * (boxExtents.m_x * (capsuleToBox.m_x > 0 ? 1.f : -1.f));
            closestPoint = closestPoint + Vector3::up() * capsuleHeight * 0.5f * (capsuleToBox.m_y > 0 ? 1.f : -1.f);

            if (closestPoint.m_y < boxMin)
                closestPoint.m_y = boxMin;
            else if (closestPoint.m_y > boxMax)
                closestPoint.m_y = boxMax;
        }

        // Check if the closest point on the box is inside the capsule
        const float dist = (closestPoint - capsuleCenter).magnitudeSquared();
        return dist < capsuleRadius * capsuleRadius;
    }

    Vector3 BoxCollider::getClosestPoint(const Vector3& point) const
    {
        const auto [center, size, _] = getBounds();
        const auto min = center - (size / 2.f);
        const auto max = center + (size / 2.f);

        return clamp(point, min, max);
    }

    Vector3 BoxCollider::getClosestPointOnSurface(const Vector3& point) const
    {
        const auto [center, size, _] = getBounds();
        const auto minCorner = center - (size / 2.f);
        const auto maxCorner = center + (size / 2.f);

        if (!check(point))
            return clamp(point, minCorner, maxCorner);

        const Vector3 snappedX
        {
            snap(point.m_x, minCorner.m_x, maxCorner.m_x),
            point.m_y,
            point.m_z
        };

        const Vector3 snappedY
        {
            point.m_x,
            snap(point.m_y, minCorner.m_y, maxCorner.m_y),
            point.m_z
        };

        const Vector3 snappedZ
        {
            point.m_x,
            point.m_y,
            snap(point.m_z, minCorner.m_z, maxCorner.m_z)
        };

        const float minDist = min(point.distanceSquaredFrom(snappedX),
            min(point.distanceSquaredFrom(snappedY),
                point.distanceSquaredFrom(snappedZ)));

        return floatEquals(minDist, point.distanceSquaredFrom(snappedX))
                   ? snappedX
                   : floatEquals(minDist, point.distanceSquaredFrom(snappedY))
                         ? snappedY
                         : snappedZ;
    }

    Bounds BoxCollider::calculateBounds(const Vector3& center, const Vector3& size)
    {
        return { center, size, (size / 2.f).magnitude() };
    }
}

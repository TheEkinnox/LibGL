#pragma once
#include "Component.h"
#include "ECollisionDetectionMode.h"
#include "EForceMode.h"
#include "Vector/Vector3.h"

#include <cstdint>

namespace LibGL::Physics
{
    class ICollider;

    class Rigidbody final : public Component
    {
    public:
        inline static LibMath::Vector3 s_gravity{0.f, -9.8f, 0.f};
        inline static float            s_friction = .4f;
        inline static uint8_t          s_continuousCollisionSteps = 8;

        LibMath::Vector3 m_velocity = LibMath::Vector3::zero();

        ECollisionDetectionMode m_collisionDetectionMode = ECollisionDetectionMode::DISCRETE;
        float                   m_sleepThreshold = 0.005f;
        float                   m_drag = 0.f;
        float                   m_mass = 1.f;
        bool                    m_useGravity = true;
        bool                    m_isKinematic = false;

        explicit Rigidbody(Entity& owner);

        void update() override;

        void addForce(const LibMath::Vector3& force, EForceMode forceMode = EForceMode::FORCE);

        void sleep();

        void wakeUp();

        bool isSleeping() const;

        LibMath::Vector3 getDraggedVelocity() const;

    private:
        bool m_isSleeping = false;

        void simulate();
        void move();

        static LibMath::Vector3 getBoundsNormal(const ICollider& entityCollider, const ICollider& worldCollider);
    };
}

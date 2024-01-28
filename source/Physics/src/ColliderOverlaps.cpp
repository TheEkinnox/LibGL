#include "ColliderOverlaps.h"

#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "Entity.h"
#include "ICollider.h"
#include "SphereCollider.h"

#include <algorithm>

using namespace LibMath;

namespace LibGL::Physics
{
    std::vector<ICollider*> overlapBox(const Vector3& center, const Vector3& size)
    {
        Entity                  tmpEntity(nullptr, { Vector3::zero(), Vector3::zero(), Vector3::one() });
        const BoxCollider       tmpCollider(tmpEntity, center, size);
        std::vector<ICollider*> colliders;

        for (const auto& worldCollider : ICollider::getColliders())
        {
            if (worldCollider != nullptr &&
                worldCollider != &tmpCollider &&
                worldCollider->isActive() &&
                tmpCollider.check(*worldCollider))
                colliders.push_back(worldCollider);
        }

        return colliders;
    }

    std::vector<ICollider*> overlapSphere(const Vector3& center, const float radius)
    {
        Entity                  tmpEntity(nullptr, { Vector3::zero(), Vector3::zero(), Vector3::one() });
        const SphereCollider    tmpCollider(tmpEntity, center, radius);
        std::vector<ICollider*> colliders;

        for (const auto& worldCollider : ICollider::getColliders())
        {
            if (worldCollider != nullptr &&
                worldCollider != &tmpCollider &&
                worldCollider->isActive() &&
                tmpCollider.check(*worldCollider))
                colliders.push_back(worldCollider);
        }

        return colliders;
    }

    std::vector<ICollider*> overlapCapsule(const Vector3& center, const Vector3& up, const float height, const float radius)
    {
        Entity                  tmpEntity(nullptr, { Vector3::zero(), Vector3::zero(), Vector3::one() });
        const CapsuleCollider   tmpCollider(tmpEntity, center, up, height, radius);
        std::vector<ICollider*> colliders;

        for (const auto& worldCollider : ICollider::getColliders())
        {
            if (worldCollider != nullptr && worldCollider != &tmpCollider && worldCollider->isActive()
                && tmpCollider.check(*worldCollider))
                colliders.push_back(worldCollider);
        }

        return colliders;
    }
}

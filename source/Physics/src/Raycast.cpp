#include "Raycast.h"

#include "Arithmetic.h"
#include "ICollider.h"

using namespace LibMath;

namespace LibGL::Physics
{
	bool raycast(const Vector3& origin, const Vector3& direction,
		const float maxDistance)
	{
		RaycastHit discard;
		return raycast(origin, direction, discard, maxDistance);
	}

	bool raycast(const Vector3& origin, const Vector3& direction,
		RaycastHit& hitInfo, const float maxDistance)
	{
		const Vector3 dir = direction.normalized();
		const auto colliders = ICollider::getColliders();
		const Ray ray{ origin, dir };
		const float maxDistanceSqr = maxDistance * maxDistance;

		for (const auto& collider : colliders)
		{
			if (collider == nullptr || !collider->isActive())
				continue;

			const auto closestOnCollider = collider->getClosestPoint(origin);
			const float distanceSqr = origin.distanceSquaredFrom(closestOnCollider);

			if (distanceSqr > maxDistanceSqr || distanceSqr >= hitInfo.m_distance ||
				!collider->check(ray, hitInfo.m_distance) || hitInfo.m_distance > maxDistanceSqr)
				continue;

			hitInfo.m_collider = collider;
		}

		if (hitInfo.m_collider != nullptr)
		{
			hitInfo.m_distance = squareRoot(hitInfo.m_distance);
			hitInfo.m_position = origin + dir * hitInfo.m_distance;
		}
		else
		{
			hitInfo.m_distance = INFINITY;
			hitInfo.m_position = Vector3(INFINITY);
		}

		return hitInfo.m_collider != nullptr;
	}
}

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

			const auto closestOnSurface = collider->getClosestPointOnSurface(origin);
			const auto closestOnRay = ray.getClosestPoint(closestOnSurface);
			const float distanceSqr = origin.distanceSquaredFrom(closestOnRay);

			if (distanceSqr > maxDistanceSqr || distanceSqr >= hitInfo.m_distance ||
				!collider->check(ray))
				continue;

			hitInfo.m_collider = collider;
			hitInfo.m_distance = distanceSqr;
		}

		if (hitInfo.m_collider != nullptr)
		{
			hitInfo.m_distance = squareRoot(hitInfo.m_distance);
			hitInfo.m_position = origin + dir * hitInfo.m_distance;
		}

		return hitInfo.m_collider != nullptr;
	}
}

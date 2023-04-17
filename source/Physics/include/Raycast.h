#pragma once
#include "Vector/Vector3.h"

namespace LibGL::Physics
{
	class ICollider;

	struct RaycastHit
	{
		LibMath::Vector3	m_position = LibMath::Vector3(INFINITY);
		ICollider*			m_collider = nullptr;
		float				m_distance = INFINITY;
	};

	/**
	 * \brief Checks collisions for a ray of the given length, from the given point,
	 * in the given direction, against all active colliders.
	 * \param origin The starting point of the ray in world coordinates
	 * \param direction The direction of the ray
	 * \param maxDistance The max distance the ray should check for collisions
	 * \return True when the ray intersects with a collider. False otherwise.
	 */
	bool raycast(const LibMath::Vector3& origin, const LibMath::Vector3& direction, float maxDistance = INFINITY);

	/**
	 * \brief Checks collisions for a ray of the given length, from the given point,
	 * in the given direction, against all active colliders.
	 * \param origin The starting point of the ray in world coordinates
	 * \param direction The direction of the ray
	 * \param hitInfo A reference to the object in which the raycast hit information should be output
	 * \param maxDistance The max distance the ray should check for collisions
	 * \return True when the ray intersects with a collider. False otherwise.
	 */
	bool raycast(const LibMath::Vector3& origin, const LibMath::Vector3& direction, RaycastHit& hitInfo, float maxDistance = INFINITY);
}

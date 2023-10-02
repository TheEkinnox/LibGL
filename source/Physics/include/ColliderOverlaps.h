#pragma once
#include <vector>

#include "Vector/Vector3.h"

namespace LibGL::Physics
{
	class ICollider;

	/**
	 * \brief Gets all active colliders overlapping the given box.
	 * \param center The center of the box
	 * \param size The size of the box
	 * \return A list of all colliders overlapping the box
	 */
	std::vector<ICollider*> overlapBox(const LibMath::Vector3& center,
		const LibMath::Vector3& size);

	/**
	 * \brief Gets all active colliders overlapping the given sphere.
	 * \param center The center of the sphere
	 * \param radius The radius of the sphere
	 * \return A list of all colliders overlapping the sphere
	 */
	std::vector<ICollider*> overlapSphere(const LibMath::Vector3& center,
		float radius);

	/**
	 * \brief Gets all active colliders overlapping the given capsule.
	 * \param center The center of the capsule
	 * \param up The up direction of the capsule
	 * \param height The height of the capsule
	 * \param radius The radius of the capsule
	 * \return A list of all colliders overlapping the capsule
	 */
	std::vector<ICollider*> overlapCapsule(const LibMath::Vector3& center,
		const LibMath::Vector3& up, float height, float radius);
}

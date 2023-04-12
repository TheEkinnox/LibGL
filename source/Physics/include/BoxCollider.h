#pragma once
#include "ICollider.h"
#include "Vector/Vector3.h"

namespace LibGL
{
	class Entity;
}

namespace LibGL::Physics
{
	class CapsuleCollider;
	class SphereCollider;

	class BoxCollider final : public ICollider
	{
	public:
		BoxCollider(Entity& owner, const LibMath::Vector3& center, const LibMath::Vector3& size);

		/**
		 * \brief Checks if a given point is colliding with the box collider.
		 * \param point The point to check collision for.
		 * \return True if the point is colliding with the box collider.
		 * False otherwise.
		 */
		bool check(const LibMath::Vector3& point) const override;

		/**
		 * \brief Checks if a given collider is colliding with the box collider.
		 * \param other The collider to check collision for.
		 * \return True if the collider is colliding with the box collider.
		 * False otherwise.
		 */
		bool check(const ICollider& other) const override;

		/**
		 * \brief Checks if a given box collider is colliding with the current box collider.
		 * \param other The box collider to check collision for.
		 * \return True if the box collider is colliding with the current box collider.
		 * False otherwise.
		 */
		bool checkBox(const BoxCollider& other) const;

		/**
		 * \brief Checks if a given sphere collider is colliding with the box collider.
		 * \param other The sphere collider to check collision for.
		 * \return True if the sphere collider is colliding with the box collider.
		 * False otherwise.
		 */
		bool checkSphere(const SphereCollider& other) const;

		/**
		 * \brief Checks if a given capsule collider is colliding with the box collider.
		 * \param other The capsule collider to check collision for.
		 * \return True if the capsule collider is colliding with the box collider.
		 * False otherwise.
		 */
		bool checkCapsule(const CapsuleCollider& other) const;

	private:
		LibMath::Vector3	m_center;
		LibMath::Vector3	m_size;
	};
}

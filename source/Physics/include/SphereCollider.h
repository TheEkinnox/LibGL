#pragma once
#include "ICollider.h"
#include "Vector/Vector3.h"

namespace LibGL
{
	class Entity;
}

namespace LibGL::Physics
{
	class BoxCollider;
	class CapsuleCollider;

	class SphereCollider final : public ICollider
	{
	public:
		SphereCollider(Entity& owner, const LibMath::Vector3& center, float radius);

		/**
		 * \brief Checks if a given point is colliding with the sphere collider.
		 * \param point The point to check collision for.
		 * \return True if the point is colliding with the sphere collider. False otherwise.
		 */
		bool check(const LibMath::Vector3& point) const override;

		/**
		 * \brief Checks if a given ray is colliding with the collider.
		 * \param ray The ray to check collision for.
		 * \param distanceSqr The squared distance from the origin to the closest intersection point. Infinity if no intersection
		 * \return True if the ray is colliding with the collider. False otherwise.
		 */
		bool check(const Ray& ray, float& distanceSqr) const override;

		/**
		 * \brief Checks if a given collider is colliding with the sphere collider.
		 * \param other The collider to check collision for.
		 * \return True if the collider is colliding with the sphere collider. False otherwise.
		 */
		bool check(const ICollider& other) const override;

		/**
		 * \brief Checks if a given box collider is colliding with the sphere collider.
		 * \param other The box collider to check collision for.
		 * \return True if the box collider is colliding with the sphere collider. False otherwise.
		 */
		bool checkBox(const BoxCollider& other) const;

		/**
		 * \brief Checks if a given sphere collider is colliding with the current sphere collider.
		 * \param other The sphere collider to check collision for.
		 * \return True if the sphere collider is colliding with the current sphere collider. False otherwise.
		 */
		bool checkSphere(const SphereCollider& other) const;

		/**
		 * \brief Checks if a given capsule collider is colliding with the sphere collider.
		 * \param other The capsule collider to check collision for.
		 * \return True if the capsule collider is colliding with the sphere collider. False otherwise.
		 */
		bool checkCapsule(const CapsuleCollider& other) const;

		/**
		 * \brief Computes the closest point to the given position inside the collider
		 * \param point The point of which we want the closest in-bounds point
		 * \return The closest point to the given position in the collider
		 */
		LibMath::Vector3 getClosestPoint(const LibMath::Vector3& point) const override;

		/**
		 * \brief Computes the closest point to the given position on the surface of the collider
		 * \param point The point of which we want the closest on-surface point
		 * \return The closest point to the given position on the surface of the collider
		 */
		LibMath::Vector3 getClosestPointOnSurface(const LibMath::Vector3& point) const override;

	private:
		LibMath::Vector3	m_center;
		float				m_radius;
	};
}

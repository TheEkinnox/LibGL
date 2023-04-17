#pragma once
#include <vector>

#include "Component.h"
#include "Vector/Vector3.h"

namespace LibGL
{
	class Entity;
}

namespace LibGL::Physics
{
	struct Ray
	{
		LibMath::Vector3	m_origin;
		LibMath::Vector3	m_direction;

		/**
		 * \brief Gets the closest point on the ray from the given position
		 * \param point The point to check against
		 * \return The closest point to the given position on the ray
		 */
		LibMath::Vector3 getClosestPoint(const LibMath::Vector3& point) const;

		/**
		 * \brief Gets the closest points between the given and current rays
		 * \param other The ray to check against
		 * \return The closest points between the given and current rays
		 */
		std::pair<LibMath::Vector3, LibMath::Vector3> getClosestPoints(const Ray& other) const;

		/**
		 * \brief Calculates the squared distance between the ray and the given point
		 * \param point The point to check against
		 * \return The squared distance between the ray and the given point
		 */
		float	distanceSquaredFrom(const LibMath::Vector3& point) const;

		/**
		 * \brief Calculates the squared distance between the current and received ray
		 * \param other The ray to check against
		 * \return The squared distance between the current and received ray
		 */
		float	distanceSquaredFrom(const Ray& other) const;
	};

	struct Bounds
	{
		LibMath::Vector3	m_center;
		LibMath::Vector3	m_boxSize;
		float				m_sphereRadius;
	};

	class ICollider : public Component
	{
	public:
		virtual ~ICollider() override;

		/**
		 * \brief Gets the collider's bounding data in world space
		 * \return The collider's bounds in world space
		 */
		Bounds getBounds() const;

		/**
		 * \brief Checks if a given point is colliding with the collider.
		 * \param point The point to check collision for.
		 * \return True if the point is colliding with the collider.
		 * False otherwise.
		 */
		virtual bool check(const LibMath::Vector3& point) const;

		/**
		 * \brief Checks if a given ray is colliding with the collider.
		 * \param ray The ray to check collision for.
		 * \return True if the ray is colliding with the collider.
		 * False otherwise.
		 */
		virtual bool check(const Ray& ray) const;

		/**
		 * \brief Checks if a collider is colliding with the current collider.
		 * \param other The collider to check collision for.
		 * \return True if the other collider is colliding with the current collider.
		 * False otherwise.
		 */
		virtual bool check(const ICollider& other) const;

		/**
		 * \brief Computes the closest point to the given position inside the collider
		 * \param point The point of which we want the closest in-bounds point
		 * \return The closest point to the given position in the collider
		 */
		virtual LibMath::Vector3 getClosestPoint(const LibMath::Vector3& point) const = 0;

		/**
		 * \brief Computes the closest point to the given position on the surface of the collider
		 * \param point The point of which we want the closest on-surface point
		 * \return The closest point to the given position on the surface of the collider
		 */
		virtual LibMath::Vector3 getClosestPointOnSurface(const LibMath::Vector3& point) const = 0;

		/**
		 * \brief Gets a list of all loaded colliders
		 * \return A list of all loaded colliders
		 */
		static std::vector<ICollider*> getColliders();

	protected:
		ICollider(Entity& owner, const Bounds& bounds);

	private:
		inline static std::vector<ICollider*> m_colliders{};

		Bounds	m_bounds;
	};

	LibMath::Vector3 getClosestPointOnSegment(const LibMath::Vector3& point,
		const LibMath::Vector3& lineStart, const LibMath::Vector3& lineEnd);
}

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
	struct Bounds
	{
		LibMath::Vector3 m_center;
		LibMath::Vector3 m_size;

		float getSphereRadius() const;
	};

	class ICollider : public Component
	{
	public:
		virtual ~ICollider() override;

		Bounds getBounds() const;

		/**
		 * \brief Checks if a given point is colliding with the collider.
		 * \param point The point to check collision for.
		 * \return True if the point is colliding with the collider.
		 * False otherwise.
		 */
		virtual bool check(const LibMath::Vector3& point) const = 0;

		/**
		 * \brief Checks if a collider is colliding with the current collider.
		 * \param other The collider to check collision for.
		 * \return True if the other collider is colliding with the current collider.
		 * False otherwise.
		 */
		virtual bool check(const ICollider& other) const;

		static std::vector<const ICollider*> getColliders();

	protected:
		ICollider(Entity& owner, const Bounds& bounds);

	private:
		inline static std::vector<ICollider*> m_colliders{};

		Bounds	m_bounds;
	};

	LibMath::Vector3 getClosestPointOnSegment(const LibMath::Vector3& point,
		const LibMath::Vector3& lineStart, const LibMath::Vector3& lineEnd);
}

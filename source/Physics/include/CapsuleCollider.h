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
    class SphereCollider;

    class CapsuleCollider final : public ICollider
    {
    public:
        CapsuleCollider(Entity& owner, const LibMath::Vector3& center, const LibMath::Vector3& upDir, float height, float radius);

        /**
         * \brief Gets the capsule collider's up direction vector
         * \return The capsule collider's up direction
         */
        LibMath::Vector3 getUpDirection() const;

        /**
         * \brief Gets the capsule collider's world space radius
         * \return The capsule collider's world space radius
         */
        float getRadius() const;

        /**
         * \brief Gets the capsule collider's world space height
         * \return The capsule collider's world space height
         */
        float getHeight() const;

        /**
         * \brief Checks if a given point is colliding with the capsule collider.
         * \param point The point to check collision for.
         * \return True if the point is colliding with the capsule collider.
         * False otherwise.
         */
        bool check(const LibMath::Vector3& point) const override;

        /**
         * \brief Checks if a given ray is colliding with the collider.
         * \param ray The ray to check collision for.
         * \param distanceSqr The squared distance from the origin to the closest intersection point
         * Infinity if no intersection
         * \return True if the ray is colliding with the collider.
         * False otherwise.
         */
        bool check(const Ray& ray, float& distanceSqr) const override;

        /**
         * \brief Checks if a given collider is colliding with the capsule collider.
         * \param other The collider to check collision for.
         * \return True if the collider is colliding with the capsule collider.
         * False otherwise.
         */
        bool check(const ICollider& other) const override;

        /**
         * \brief Checks if a given box collider is colliding with the capsule collider.
         * \param other The box collider to check collision for.
         * \return True if the box collider is colliding with the capsule collider.
         * False otherwise.
         */
        bool checkBox(const BoxCollider& other) const;

        /**
         * \brief Checks if a given sphere collider is colliding with the capsule collider.
         * \param other The sphere collider to check collision for.
         * \return True if the sphere collider is colliding with the capsule collider.
         * False otherwise.
         */
        bool checkSphere(const SphereCollider& other) const;

        /**
         * \brief Checks if a given capsule collider is colliding with the current capsule collider.
         * \param other The capsule collider to check collision for.
         * \return True if the capsule collider is colliding with the current capsule collider.
         * False otherwise.
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
        LibMath::Vector3 m_center = LibMath::Vector3::zero();
        LibMath::Vector3 m_upDirection = LibMath::Vector3::up();
        float            m_height = 1.f;
        float            m_radius = .5f;

        /**
         * \brief Calculates the bounds of the capsule collider with the given values
         * \param center The center of the capsule collider
         * \param upDir The up direction vector of the capsule collider
         * \param height The height of the capsule collider
         * \param radius The radius of the capsule collider
         * \return The bounds of the capsule collider
         */
        static Bounds calculateBounds(const LibMath::Vector3& center, const LibMath::Vector3& upDir, float height, float radius);
    };
}

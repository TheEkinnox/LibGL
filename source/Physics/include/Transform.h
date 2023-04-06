#pragma once
#include "Matrix/Matrix4.h"
#include "Vector/Vector3.h"

namespace LibGL::Physics
{
	class Transform
	{
	public:
		/**
		 * \brief Creates a transform with no translation, no rotation
		 * and a scale of 1
		 */
		Transform();

		/**
		 * \brief Creates a transform with the given position, rotation
		 * and scale
		 * \param position The transform's initial position
		 * \param rotation The transform's initial rotation
		 * \param scale The transform's initial scale
		 */
		Transform(const LibMath::Vector3& position, const LibMath::Vector3& rotation, const LibMath::Vector3& scale);

		/**
		 * \brief Creates a copy of the given transform
		 * \param other The transform to copy
		 */
		Transform(const Transform& other);

		/**
		 * \brief Creates a move copy of the given transform
		 * \param other The transform to move
		 */
		Transform(Transform&& other) noexcept;

		/**
		 * \brief Destroys the transform
		 */
		virtual ~Transform() = default;

		/**
		 * \brief Copies the given transform's data into the current one
		 * \param other The transform to copy
		 * \return A reference to the current transform
		 */
		Transform& operator=(const Transform& other);

		/**
		 * \brief Moves the given transform's data into the current one
		 * \param other The transform to move
		 * \return A reference to the current transform
		 */
		Transform& operator=(Transform&& other) noexcept;

		/**
		 * \brief Computes the transform's forward vector
		 * \return The transform's forward vector
		 */
		LibMath::Vector3 forward() const;

		/**
		 * \brief Computes the transform's right vector
		 * \return The transform's right vector
		 */
		LibMath::Vector3 right() const;

		/**
		 * \brief Computes the transform's up vector
		 * \return The transform's up vector
		 */
		LibMath::Vector3 up() const;

		/**
		 * \brief Computes the transform's back vector
		 * \return The transform's back vector
		 */
		LibMath::Vector3 back() const;

		/**
		 * \brief Computes the transform's left vector
		 * \return The transform's left vector
		 */
		LibMath::Vector3 left() const;

		/**
		 * \brief Computes the transform's down vector
		 * \return The transform's down vector
		 */
		LibMath::Vector3 down() const;

		/**
		 * \brief Gets the transform's current position
		 * \return The transform's position
		 */
		LibMath::Vector3 getPosition() const;

		/**
		 * \brief Gets the transform's current rotation
		 * \return The transform's rotation
		 */
		LibMath::Vector3 getRotation() const;

		/**
		 * \brief Gets the transform's current scale
		 * \return The transform's scale
		 */
		LibMath::Vector3 getScale() const;

		/**
		 * \brief Gets the transform's current transformation matrix
		 * \return The transform's transformation matrix
		 */
		LibMath::Matrix4x4 getMatrix() const;

		/**
		 * \brief Sets the transform's current position
		 * \param position The transform's new position
		 * \return A reference to the current transform
		 */
		Transform& setPosition(const LibMath::Vector3& position);

		/**
		 * \brief Sets the transform's current rotation
		 * \param rotation The transform's new rotation
		 * \return A reference to the current transform
		 */
		Transform& setRotation(const LibMath::Vector3& rotation);

		/**
		 * \brief Sets the transform's current rotation
		 * \param scale The transform's new scale
		 * \return A reference to the current transform
		 */
		Transform& setScale(const LibMath::Vector3& scale);

		/**
		 * \brief Adds the given vector to the transform's current position
		 * \param translation The translation vector to apply
		 * \return A reference to the current transform
		 */
		Transform& translate(const LibMath::Vector3& translation);

		/**
		 * \brief Adds the given vector to the transform's current rotation
		 * \param rotation The rotation vector to apply
		 * \return A reference to the current transform
		 */
		Transform& rotate(const LibMath::Vector3& rotation);

		/**
		 * \brief Multiplies the transform's current scale by the given vector
		 * \param scale The scaling vector to apply
		 * \return A reference to the current transform
		 */
		Transform& scale(const LibMath::Vector3& scale);

	protected:
		/**
		 * \brief Updates the transform's matrix
		 */
		virtual void onChange();

	private:
		LibMath::Vector3	m_position;
		LibMath::Vector3	m_rotation;
		LibMath::Vector3	m_scale;
		LibMath::Matrix4	m_matrix;
	};;
}

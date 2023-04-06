#pragma once
#include "Resources/Scene.h"

namespace LibGL::Rendering
{
	class Camera : public Resources::SceneNode
	{
	public:
		/**
		 * \brief Creates an orthographic camera with no transformation
		 */
		Camera();

		/**
		 * \brief Creates a camera with the given transform and projection matrix
		 * \param parent The camera's parent in the scene graph (nullptr if at the root)
		 * \param transform The camera's initial transform
		 * \param projectionMatrix The camera's projection matrix
		 */
		explicit Camera(SceneNode* parent, const Transform& transform, LibMath::Matrix4 projectionMatrix);

		/**
		 * \brief Creates a copy of a given camera
		 * \param other The camera to copy
		 */
		Camera(const Camera& other);

		/**
		 * \brief Creates a move copy of a given camera
		 * \param other The camera to move
		 */
		Camera(Camera&& other) noexcept;

		/**
		 * \brief Destroys the camera
		 */
		~Camera() override = default;

		/**
		 * \brief Assigns a copy of the given camera to the current one
		 * \param other The camera to copy
		 * \return A reference to the current camera
		 */
		Camera& operator=(const Camera& other);

		/**
		 * \brief Assigns a move copy of the given camera to the current one
		 * \param other The camera to move
		 * \return A reference to the current camera
		 */
		Camera& operator=(Camera&& other) noexcept;

		/**
		 * \brief Gets the camera's view matrix
		 * \return The camera's view matrix
		 */
		LibMath::Matrix4 getViewMatrix() const;

		/**
		 * \brief Gets the camera's projection matrix
		 * \return The camera's projection matrix
		 */
		LibMath::Matrix4 getViewProjectionMatrix() const;

		/**
		 * \brief Makes the camera use the given projection matrix
		 * \param projectionMatrix The camera's new projection matrix
		 * \return A reference to the current camera
		 */
		Camera& setProjectionMatrix(const LibMath::Matrix4& projectionMatrix);

		/**
		 * \brief Gets a reference to the currently rendering camera
		 * \return A reference to the current camera
		 */
		static Camera& getCurrent();

		/**
		 * \brief Sets the currently rendering camera
		 * \param cam The currently rendering camera
		 */
		static void setCurrent(Camera& cam);

	private:
		static Camera*		m_current;

		LibMath::Matrix4	m_viewMatrix;
		LibMath::Matrix4	m_projectionMatrix;
		LibMath::Matrix4	m_viewProjectionMatrix;

		/**
		 * \brief Updates the camera's matrices and other cached information
		 */
		void onChange() override;

		/**
		 * \brief Updates the camera's matrices
		 */
		void updateMatrices();
	};
}

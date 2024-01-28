#pragma once
#include "Entity.h"
#include "Transform.h"
#include "Core/Color.h"
#include "Matrix/Matrix4.h"

namespace LibGL::Rendering
{
    class Camera : public Entity
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
        explicit Camera(Entity* parent, const LibMath::Transform& transform, LibMath::Matrix4 projectionMatrix);

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
        ~Camera() override;

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
         * \brief Sets the camera's clear color
         * \param color The new clear color
         * \return A reference to the current camera
         */
        Camera& setClearColor(const Color& color);

        /**
         * \brief Sets the camera's clear color
         * \param r The clear color's red component
         * \param g The clear color's green component
         * \param b The clear color's blue component
         * \param a The clear color's alpha component
         * \return A reference to the current camera
         */
        Camera& setClearColor(float r, float g, float b, float a = 1.f);

        /**
         * \brief Gets the camera's current clear color
         * \return The camera's current clear color
         */
        Color getClearColor() const;

        /**
         * \brief  Sets whether the color buffer should be cleared
         * \param enable Whether the color buffer should be cleared
         * \return A reference to the current camera
         */
        Camera& setClearColorBuffer(bool enable);

        /**
         * \brief Checks if the color buffer should be cleared
         * \return True if the color buffer should be cleared. False otherwise.
         */
        bool getClearColorBuffer() const;

        /**
         * \brief  Sets whether the depth buffer should be cleared
         * \param enable Whether the depth buffer should be cleared
         * \return A reference to the current camera
         */
        Camera& setClearDepthBuffer(bool enable);

        /**
         * \brief Checks if the depth buffer should be cleared
         * \return True if the depth buffer should be cleared. False otherwise.
         */
        bool getClearDepthBuffer() const;

        /**
         * \brief Sets whether the stencil buffer should be cleared
         * \param enable Whether the stencil buffer should be cleared
         * \return A reference to the current camera
         */
        Camera& setClearStencilBuffer(bool enable);

        /**
         * \brief Checks if the stencil buffer should be cleared
         * \return True if the stencil buffer should be cleared. False otherwise.
         */
        bool getClearStencilBuffer() const;

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
        static Camera* m_current;

        LibMath::Matrix4 m_viewMatrix;
        LibMath::Matrix4 m_projectionMatrix;
        LibMath::Matrix4 m_viewProjectionMatrix;
        Color            m_clearColor         = Color::black;
        bool             m_clearColorBuffer   = true;
        bool             m_clearDepthBuffer   = true;
        bool             m_clearStencilBuffer = true;

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

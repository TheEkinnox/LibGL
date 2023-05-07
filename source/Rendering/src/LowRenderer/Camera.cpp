#include "LowRenderer/Camera.h"

using namespace LibMath;
using namespace LibGL::Resources;

namespace LibGL::Rendering
{
	Camera* Camera::m_current = nullptr;

	Camera::Camera()
		: Entity(),
		m_projectionMatrix(orthographicProjection(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f))
	{
		updateMatrices();
	}

	Camera::Camera(Entity* parent, const Transform& transform, Matrix4 projectionMatrix)
		: Entity(parent, transform), m_projectionMatrix(std::move(projectionMatrix))
	{
		updateMatrices();
	}

	Camera::Camera(const Camera& other)
		: Entity(other), m_projectionMatrix(other.m_projectionMatrix)
	{
		updateMatrices();
	}

	Camera::Camera(Camera&& other) noexcept
		: Entity(std::move(other)),
		m_projectionMatrix(std::move(other.m_projectionMatrix))
	{
		updateMatrices();
	}

	Camera::~Camera()
	{
		if (this == m_current)
			m_current = nullptr;
	}

	Camera& Camera::operator=(const Camera& other)
	{
		if (&other == this)
			return *this;

		m_projectionMatrix = other.m_projectionMatrix;

		updateMatrices();

		return *this;
	}

	Camera& Camera::operator=(Camera&& other) noexcept
	{
		if (&other == this)
			return *this;

		m_projectionMatrix = other.m_projectionMatrix;

		updateMatrices();

		return *this;
	}

	Matrix4 Camera::getViewMatrix() const
	{
		return m_viewMatrix;
	}

	Matrix4 Camera::getViewProjectionMatrix() const
	{
		return m_viewProjectionMatrix;
	}

	Camera& Camera::setProjectionMatrix(const Matrix4& projectionMatrix)
	{
		m_projectionMatrix = projectionMatrix;

		updateMatrices();

		return *this;
	}

	Camera& Camera::setClearColor(const Color& color)
	{
		m_clearColor = color;
		return *this;
	}

	Camera& Camera::setClearColor(const float r, const float g, const float b, const float a)
	{
		setClearColor({ r, g, b, a });
		return *this;
	}

	Color Camera::getClearColor() const
	{
		return m_clearColor;
	}

	Camera& Camera::setClearColorBuffer(const bool enable)
	{
		m_clearColorBuffer = enable;
		return *this;
	}

	bool Camera::getClearColorBuffer() const
	{
		return m_clearColorBuffer;
	}

	Camera& Camera::setClearDepthBuffer(const bool enable)
	{
		m_clearDepthBuffer = enable;
		return *this;
	}

	bool Camera::getClearDepthBuffer() const
	{
		return m_clearDepthBuffer;
	}

	Camera& Camera::setClearStencilBuffer(const bool enable)
	{
		m_clearStencilBuffer = enable;
		return *this;
	}

	bool Camera::getClearStencilBuffer() const
	{
		return m_clearStencilBuffer;
	}

	Camera& Camera::getCurrent()
	{
		static Camera defaultCam;

		return m_current ? *m_current : defaultCam;
	}

	void Camera::setCurrent(Camera& cam)
	{
		m_current = &cam;
	}

	void Camera::onChange()
	{
		Entity::onChange();
		updateMatrices();
	}

	void Camera::updateMatrices()
	{
		const auto camCenter = getGlobalTransform().getPosition() + getGlobalTransform().forward();
		m_viewMatrix = lookAt(getGlobalTransform().getPosition(), camCenter, getGlobalTransform().up());
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}
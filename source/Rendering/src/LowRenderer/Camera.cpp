#include "LowRenderer/Camera.h"

using namespace LibMath;
using namespace LibGL::Resources;
using namespace LibGL::Physics;

namespace LibGL::Rendering
{
	Camera* Camera::m_current = nullptr;

	Camera::Camera()
		: SceneNode(),
		m_projectionMatrix(Matrix4::orthographicProjection(-1, 1, -1, 1, -1, 1))
	{
		updateMatrices();
	}

	Camera::Camera(SceneNode* parent, const Transform& transform, Matrix4 projectionMatrix)
		: SceneNode(parent, transform), m_projectionMatrix(std::move(projectionMatrix))
	{
		updateMatrices();
	}

	Camera::Camera(const Camera& other)
		: SceneNode(other), m_projectionMatrix(other.m_projectionMatrix)
	{
		updateMatrices();
	}

	Camera::Camera(Camera&& other) noexcept
		: SceneNode(std::move(other)),
		m_projectionMatrix(std::move(other.m_projectionMatrix))
	{
		updateMatrices();
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

		m_projectionMatrix = std::move(other.m_projectionMatrix);

		updateMatrices();

		return *this;
	}

	LibMath::Matrix4 Camera::getViewMatrix() const
	{
		return m_viewMatrix;
	}

	LibMath::Matrix4 Camera::getViewProjectionMatrix() const
	{
		return m_viewProjectionMatrix;
	}

	Camera& Camera::setProjectionMatrix(const Matrix4& projectionMatrix)
	{
		m_projectionMatrix = projectionMatrix;

		updateMatrices();

		return *this;
	}

	Camera& Camera::getCurrent()
	{
		return *m_current;
	}

	void Camera::setCurrent(Camera& cam)
	{
		m_current = &cam;
	}

	void Camera::onChange()
	{
		SceneNode::onChange();
		updateMatrices();
	}

	void Camera::updateMatrices()
	{
		const auto camCenter = getGlobalTransform().getPosition() + getGlobalTransform().forward();
		m_viewMatrix = Matrix4::lookAt(getGlobalTransform().getPosition(), camCenter, Vector3::up());
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}
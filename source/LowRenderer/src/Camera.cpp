#include "Camera.h"

My::Rendering::Camera::Camera()
	: Transform(),
	m_projectionMatrix(Matrix4::orthographicProjection(-1, 1, -1, 1, -1, 1))
{
	updateMatrices();
}

My::Rendering::Camera::Camera(const Transform& transform, Matrix4 projectionMatrix)
	: Transform(transform), m_projectionMatrix(std::move(projectionMatrix))
{
	updateMatrices();
}

My::Rendering::Camera::Camera(const Camera& other)
	: Transform(other), m_projectionMatrix(other.m_projectionMatrix)
{
	updateMatrices();
}

My::Rendering::Camera::Camera(Camera&& other) noexcept
	: Transform(std::move(other)),
	  m_projectionMatrix(std::move(other.m_projectionMatrix))
{
	updateMatrices();
}

My::Rendering::Camera& My::Rendering::Camera::operator=(const Camera& other)
{
	if (&other == this)
		return *this;
	
	m_projectionMatrix = other.m_projectionMatrix;

	updateMatrices();

	return *this;
}

My::Rendering::Camera& My::Rendering::Camera::operator=(Camera&& other) noexcept
{
	if (&other == this)
		return *this;
	
	m_projectionMatrix = std::move(other.m_projectionMatrix);

	updateMatrices();

	return *this;
}

LibMath::Matrix4 My::Rendering::Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

LibMath::Matrix4 My::Rendering::Camera::getViewProjectionMatrix() const
{
	return m_viewProjectionMatrix;
}

My::Rendering::Camera& My::Rendering::Camera::setProjectionMatrix(const Matrix4& projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;

	updateMatrices();

	return *this;
}

void My::Rendering::Camera::onChange()
{
	Transform::onChange();
	updateMatrices();
}

void My::Rendering::Camera::updateMatrices()
{
	m_viewMatrix = getMatrix().inverse();
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

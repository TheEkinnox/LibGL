#pragma once
#include "Transform.h"

namespace My::Rendering
{
	using namespace Physics;

	class Camera : public Transform
	{
	public:
					Camera();
		explicit	Camera(const Transform& transform, Matrix4 projectionMatrix);
					Camera(const Camera& other);
					Camera(Camera&& other) noexcept;
					~Camera() override = default;

		Camera&		operator=(const Camera& other);
		Camera&		operator=(Camera&& other) noexcept;

		Matrix4		getViewMatrix() const;
		Matrix4		getViewProjectionMatrix() const;

		Camera&		setProjectionMatrix(const Matrix4& projectionMatrix);

	private:
		Matrix4		m_viewMatrix;
		Matrix4		m_projectionMatrix;
		Matrix4		m_viewProjectionMatrix;

		void		onChange() override;
		void		updateMatrices();
	};
}

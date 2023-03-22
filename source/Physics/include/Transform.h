#pragma once
#include "Maths/Matrix/Matrix4.h"
#include "Maths/Vector/Vector3.h"

namespace My::Physics
{
	using namespace LibMath;

	class Transform
	{
	public:
						Transform();
						Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale);
						Transform(const Transform& other);
						Transform(Transform&& other) noexcept;
		virtual			~Transform() = default;

		Transform&		operator=(const Transform& other);
		Transform&		operator=(Transform&& other) noexcept;

		Vector3			forward() const;
		Vector3			right() const;
		Vector3			up() const;

		Vector3			getPosition() const;
		Vector3			getRotation() const;
		Vector3			getScale() const;
		Matrix4x4		getMatrix() const;

		Transform&		setPosition(const Vector3& position);
		Transform&		setRotation(const Vector3& rotation);
		Transform&		setScale(const Vector3& scale);

		Transform&		translate(const Vector3& translation);
		Transform&		rotate(const Vector3& rotation);
		Transform&		scale(const Vector3& scale);

	protected:
		virtual void	onChange();

	private:
		Vector3			m_position;
		Vector3			m_rotation;
		Vector3			m_scale;
		Matrix4			m_matrix;
	};;
}

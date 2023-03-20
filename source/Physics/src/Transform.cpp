#include "Transform.h"

#include "Maths/Trigonometry.h"
#include "Maths/Angle/Degree.h"

namespace My::Physics
{
	Transform::Transform()
		: m_position(Vector3::zero()), m_rotation(Vector3::zero()),
		m_scale(Vector3::one())
	{
		Transform::onChange();
	}

	Transform::Transform(const Vector3& position, const Vector3& rotation,
		const Vector3& scale)
		: m_position(position), m_rotation(rotation), m_scale(scale)
	{
		Transform::onChange();
	}

	Transform::Transform(const Transform& other)
		: m_position(other.m_position), m_rotation(other.m_rotation),
		m_scale(other.m_scale)
	{
		Transform::onChange();
	}

	Transform::Transform(Transform&& other) noexcept
		: m_position(other.m_position), m_rotation(other.m_rotation),
		m_scale(other.m_scale)
	{
		Transform::onChange();
	}

	Transform& Transform::operator=(const Transform& other)
	{
		if (&other == this)
			return *this;

		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scale = other.m_scale;

		onChange();

		return *this;
	}

	Transform& Transform::operator=(Transform&& other) noexcept
	{
		if (&other == this)
			return *this;

		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scale = other.m_scale;

		onChange();

		return *this;
	}

	Vector3 Transform::forward() const
	{
		const auto xDegrees = Degree(m_rotation.m_x);
		const auto yDegrees = Degree(m_rotation.m_y);

		return Vector3
		{
			cos(xDegrees) * sin(yDegrees),
			sin(xDegrees),
			-cos(xDegrees) * cos(yDegrees)
		}.normalized();
	}

	Vector3 Transform::right() const
	{
		const auto xDegrees = Degree(m_rotation.m_x);
		const auto yDegrees = Degree(m_rotation.m_y);

		return Vector3
		{
			cos(yDegrees),
			0,
			sin(yDegrees)
		}.normalized();
	}

	Vector3 Transform::up() const
	{
		return forward().cross(right());
	}

	Vector3 Transform::getPosition() const
	{
		return m_position;
	}

	Vector3 Transform::getRotation() const
	{
		return m_rotation;
	}

	Vector3 Transform::getScale() const
	{
		return m_scale;
	}

	Matrix4x4 Transform::getMatrix() const
	{
		return m_matrix;
	}

	Transform& Transform::setPosition(const Vector3& position)
	{
		m_position = position;

		onChange();

		return *this;
	}

	Transform& Transform::setRotation(const Vector3& rotation)
	{
		m_rotation = rotation;

		onChange();

		return *this;
	}

	Transform& Transform::setScale(const Vector3& scale)
	{
		m_scale = scale;

		onChange();

		return *this;
	}

	Transform& Transform::translate(const Vector3& position)
	{
		m_position += position;

		onChange();

		return *this;
	}

	Transform& Transform::rotate(const Vector3& rotation)
	{
		m_rotation += rotation;

		onChange();

		return *this;
	}

	Transform& Transform::scale(const Vector3& scale)
	{
		m_scale *= scale;

		onChange();

		return *this;
	}

	void Transform::onChange()
	{
		m_matrix = Matrix4::scaling(m_scale)
			* Matrix4::rotation(m_rotation, false)
			* Matrix4::translation(m_position);
	}
}

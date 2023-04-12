#include "Core/Color.h"

#include "Vector/Vector3.h"
#include "Vector/Vector4.h"

namespace LibGL::Rendering
{
	Color::Color(const float r, const float g, const float b) :
		Color(r, g, b, 1)
	{
	}

	Color::Color(const float r, const float g, const float b, const float a) :
		m_r(r), m_g(g), m_b(b), m_a(a)
	{
	}

	Color::Color(const LibMath::Vector3& rgb) :
		Color(rgb.m_x, rgb.m_y, rgb.m_z)
	{
	}

	Color::Color(const LibMath::Vector3& rgb, const float a) :
		Color(rgb.m_x, rgb.m_y, rgb.m_z, a)
	{
	}

	Color::Color(const LibMath::Vector4& rgba) :
		Color(rgba.m_x, rgba.m_y, rgba.m_z, rgba.m_w)
	{
	}

	LibMath::Vector3 Color::rgb() const
	{
		return { m_r, m_g, m_b };
	}

	LibMath::Vector4 Color::rgba() const
	{
		return { m_r, m_g, m_b, m_a };
	}

	Color Color::operator+(const Color& other) const
	{
		return {
			m_r + other.m_r,
			m_g + other.m_g,
			m_b + other.m_b,
			m_a + other.m_a
		};
	}

	Color Color::operator-(const Color& other) const
	{
		return {
			m_r - other.m_r,
			m_g - other.m_g,
			m_b - other.m_b,
			m_a - other.m_a
		};
	}

	Color Color::operator*(const Color& other) const
	{
		return {
			m_r * other.m_r,
			m_g * other.m_g,
			m_b * other.m_b,
			m_a * other.m_a
		};
	}

	Color Color::operator/(const Color& other) const
	{
		return {
			m_r / other.m_r,
			m_g / other.m_g,
			m_b / other.m_b,
			m_a / other.m_a
		};
	}

	Color Color::operator*(const float scalar) const
	{
		return *this * LibMath::Vector4(scalar);
	}

	Color Color::operator/(const float scalar) const
	{
		return *this / LibMath::Vector4(scalar);
	}

	Color& Color::operator*=(const float scalar)
	{
		return *this *= LibMath::Vector4(scalar);
	}

	Color& Color::operator/=(const float scalar)
	{
		return *this /= LibMath::Vector4(scalar);
	}

	Color& Color::operator+=(const Color& other)
	{
		return (*this = *this + other);
	}

	Color& Color::operator-=(const Color& other)
	{
		return (*this = *this - other);
	}

	Color& Color::operator*=(const Color& other)
	{
		return (*this = *this * other);
	}

	Color& Color::operator/=(const Color& other)
	{
		return (*this = *this / other);
	}
}

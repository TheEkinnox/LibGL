#pragma once

namespace LibMath
{
	class Vector3;
	class Vector4;
}

namespace LibGL::Rendering
{
	struct Color
	{
		float m_r = 1;
		float m_g = 0;
		float m_b = 1;
		float m_a = 1;

		static const Color red;
		static const Color green;
		static const Color blue;
		static const Color magenta;
		static const Color black;
		static const Color white;
		static const Color clear;

		Color() = default;

		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);

		Color(const Color& other) = default;
		Color(Color&& other) = default;

		Color(const LibMath::Vector3& rgb);
		Color(const LibMath::Vector3& rgb, float a);
		Color(const LibMath::Vector4& rgba);

		~Color() = default;

		Color& operator=(const Color& other) = default;
		Color& operator=(Color&& other) = default;

		Color operator+(const Color& other) const;
		Color operator-(const Color& other) const;
		Color operator*(const Color& other) const;
		Color operator/(const Color& other) const;
		Color operator*(float scalar) const;
		Color operator/(float scalar) const;

		Color& operator+=(const Color& other);
		Color& operator-=(const Color& other);
		Color& operator*=(const Color& other);
		Color& operator/=(const Color& other);
		Color& operator*=(float scalar);
		Color& operator/=(float scalar);

		/**
		 * \brief Casts the color to a vector3
		 * \return The r, g and b components of the color as a vector3
		 */
		LibMath::Vector3 rgb() const;

		/**
		 * \brief Casts the color to a vector4
		 * \return The r, g and b and a components of the color as a vector4
		 */
		LibMath::Vector4 rgba() const;
	};

	inline const Color Color::red		= { 1.f, 0.f, 0.f, 1.f };
	inline const Color Color::green		= { 0.f, 1.f, 0.f, 1.f };
	inline const Color Color::blue		= { 0.f, 0.f, 1.f, 1.f };
	inline const Color Color::magenta	= { 1.f, 0.f, 1.f, 1.f };
	inline const Color Color::black		= { 0.f, 0.f, 0.f, 1.f };
	inline const Color Color::white		= { 1.f, 1.f, 1.f, 1.f };
	inline const Color Color::clear		= { 0.f, 0.f, 0.f, 0.f };
}


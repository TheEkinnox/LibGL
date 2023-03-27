#pragma once
#include "Maths/Vector/Vector4.h"

namespace My::Rendering
{
	using namespace LibMath;

	struct Light
	{
		Vector4	m_ambient;
		Vector4	m_diffuse;
		Vector4	m_specular;
	};

	struct DirectionalLight : Light
	{
		Vector3	m_direction;
	};

	struct PointLight : Light
	{
		Vector3	m_position;
		float	m_constant = 1;
		float	m_linear = 0;
		float	m_quadratic = 0;
	};

	struct SpotLight : Light
	{
		Vector3	m_position;
		Vector3	m_direction;

		float	m_cutOff = 0;
		float	m_outerCutoff = 0;
		float	m_constant = 1;
		float	m_linear = 0;
		float	m_quadratic = 0;
	};
}

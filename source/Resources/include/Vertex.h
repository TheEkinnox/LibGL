#pragma once
#include "Vector/Vector2.h"
#include "Vector/Vector3.h"

namespace My::Resources
{
	using namespace LibMath;

	struct Vertex
	{
		Vector3 m_position;
		Vector3 m_normal;
		Vector2 m_textureUV;

		bool operator==(const Vertex& other) const
		{
			return other.m_position == m_position
				&& other.m_textureUV == m_textureUV
				&& other.m_normal == m_normal;
		}
	};
}

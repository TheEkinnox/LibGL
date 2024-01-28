#pragma once
#include <Vector/Vector2.h>
#include <Vector/Vector3.h>

namespace LibGL::Rendering
{
    struct Vertex
    {
        LibMath::Vector3 m_position;
        LibMath::Vector3 m_normal;
        LibMath::Vector2 m_textureUV;

        bool operator==(const Vertex& other) const
        {
            return other.m_position == m_position
                && other.m_textureUV == m_textureUV
                && other.m_normal == m_normal;
        }
    };
}

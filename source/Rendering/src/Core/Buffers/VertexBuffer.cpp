#include "Core/Buffers/VertexBuffer.h"

#include <glad/gl.h>

namespace LibGL::Rendering
{
    VertexBuffer::VertexBuffer(const Vertex* vertices, const intptr_t verticesCount)
    {
        glGenBuffers(1, &m_bufferIndex);
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferIndex);
        glBufferData(GL_ARRAY_BUFFER, verticesCount * static_cast<GLsizeiptr>(sizeof(Vertex)),
            vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
        : VertexBuffer(vertices.data(), static_cast<intptr_t>(vertices.size()))
    {
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferIndex);
    }

    void VertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

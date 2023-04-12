#include "Core/Buffers/VertexBuffer.h"

#include <glad/glad.h>

using namespace LibGL::Resources;

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
	{
		const Vertex* verticesArray = vertices.data();

		glGenBuffers(1, &m_bufferIndex);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferIndex);
		glBufferData(GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(vertices.size()) * static_cast<GLsizeiptr>(sizeof(Vertex)),
			verticesArray, GL_STATIC_DRAW);
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

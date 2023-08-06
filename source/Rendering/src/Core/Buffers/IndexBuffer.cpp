#include "Core/Buffers/IndexBuffer.h"

#include <glad/glad.h>

namespace LibGL::Rendering
{
	IndexBuffer::IndexBuffer(const uint32_t* indices, const intptr_t indexCount)
	{
		glGenBuffers(1, &m_bufferIndex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferIndex);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * static_cast<GLsizeiptr>(sizeof(uint32_t)),
			indices, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices) :
		IndexBuffer(indices.data(), static_cast<intptr_t>(indices.size()))
	{
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferIndex);
	}

	void IndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

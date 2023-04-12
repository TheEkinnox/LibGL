#include "Core/Buffers/IndexBuffer.h"
#include "Resources/Vertex.h"

#include <glad/glad.h>

namespace LibGL::Rendering
{
	IndexBuffer::IndexBuffer(const uint32_t* indices, const intptr_t indexCount)
	{
		glGenBuffers(1, &m_bufferIndex);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferIndex);
		glBufferData(GL_ARRAY_BUFFER, indexCount * static_cast<GLsizeiptr>(sizeof(Resources::Vertex)),
			indices, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices)
	{
		const uint32_t* idsArray = indices.data();

		glGenBuffers(1, &m_bufferIndex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferIndex);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(indices.size()) * static_cast<GLsizeiptr>(sizeof(uint32_t)),
			idsArray, GL_STATIC_DRAW);
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

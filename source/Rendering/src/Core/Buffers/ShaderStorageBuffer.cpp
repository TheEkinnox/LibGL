#include "Core/Buffers/ShaderStorageBuffer.h"

#include <glad/glad.h>

namespace LibGL::Rendering
{
	ShaderStorageBuffer::ShaderStorageBuffer(EAccessSpecifier accessSpecifier)
	{
		glGenBuffers(1, &m_bufferIndex);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferIndex);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, static_cast<GLenum>(accessSpecifier));
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_bufferIndex);
	}

	void ShaderStorageBuffer::setBindingPoint(const uint32_t bindingPoint)
	{
		m_bindingPoint = bindingPoint;
	}

	void ShaderStorageBuffer::bind(const uint32_t bindingPoint)
	{
		setBindingPoint(bindingPoint);
		bind();
	}

	void ShaderStorageBuffer::bind() const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, m_bufferIndex);
	}

	void ShaderStorageBuffer::unbind()
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, 0);
		m_bindingPoint = 0;
	}

	void ShaderStorageBuffer::sendBlocks(const void* data, const size_t blockSize) const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferIndex);
		glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(blockSize), data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

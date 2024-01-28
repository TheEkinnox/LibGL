#include "Core/Buffers/Buffer.h"

#include <glad/gl.h>

namespace LibGL::Rendering
{
	Buffer::Buffer(Buffer&& other) noexcept
		: m_bufferIndex(other.m_bufferIndex)
	{
		other.m_bufferIndex = 0;
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &m_bufferIndex);
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		if (&other == this)
			return *this;

		glDeleteBuffers(1, &m_bufferIndex);

		m_bufferIndex = other.m_bufferIndex;

		other.m_bufferIndex = 0;

		return *this;
	}
}

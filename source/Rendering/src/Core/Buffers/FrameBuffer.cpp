#include "Core/Buffers/FrameBuffer.h"

#include <glad/glad.h>

namespace LibGL::Rendering
{
    FrameBuffer::FrameBuffer()
    {
        glGenFramebuffers(1, &m_bufferIndex);
    }

    FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
        : m_bufferIndex(other.m_bufferIndex)
    {
        other.m_bufferIndex = 0;
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &m_bufferIndex);
    }

    FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        glDeleteFramebuffers(1, &m_bufferIndex);
        m_bufferIndex = other.m_bufferIndex;
        other.m_bufferIndex = 0;

        return *this;
    }

    void FrameBuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_bufferIndex);
    }

    void FrameBuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

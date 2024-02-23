#include "Core/Buffers/FrameBuffer.h"

#include <glad/gl.h>

namespace LibGL::Rendering
{
    FrameBuffer::FrameBuffer()
    {
        glGenFramebuffers(1, &m_id);
    }

    FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
        : m_id(other.m_id)
    {
        other.m_id = 0;
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &m_id);
    }

    FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        glDeleteFramebuffers(1, &m_id);
        m_id       = other.m_id;
        other.m_id = 0;

        return *this;
    }

    void FrameBuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    }

    void FrameBuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::attach(const Resources::Texture& texture, const EFrameBufferAttachment attachmentMode) const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(attachmentMode), GL_TEXTURE_2D, texture.getId(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::detach(const EFrameBufferAttachment attachmentMode) const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(attachmentMode), GL_TEXTURE_2D, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

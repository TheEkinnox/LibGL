#include "Core/Buffers/ShaderStorageBuffer.h"

#include <glad/gl.h>

namespace LibGL::Rendering
{
    GLenum ToGLEnum(const EAccessSpecifier p_accessSpecifier)
    {
        switch (p_accessSpecifier)
        {
        case EAccessSpecifier::STREAM_DRAW:
            return GL_STREAM_DRAW;
        case EAccessSpecifier::STREAM_READ:
            return GL_STREAM_READ;
        case EAccessSpecifier::STREAM_COPY:
            return GL_STREAM_COPY;
        case EAccessSpecifier::DYNAMIC_DRAW:
            return GL_DYNAMIC_DRAW;
        case EAccessSpecifier::DYNAMIC_READ:
            return GL_DYNAMIC_READ;
        case EAccessSpecifier::DYNAMIC_COPY:
            return GL_DYNAMIC_COPY;
        case EAccessSpecifier::STATIC_DRAW:
            return GL_STATIC_DRAW;
        case EAccessSpecifier::STATIC_READ:
            return GL_STATIC_READ;
        case EAccessSpecifier::STATIC_COPY:
            return GL_STATIC_COPY;
        default:
            return GL_INVALID_ENUM;
        }
    }

    ShaderStorageBuffer::ShaderStorageBuffer(const EAccessSpecifier p_accessSpecifier, const uint32_t p_bindIndex)
        : m_bindIndex(p_bindIndex), m_accessSpecifier(p_accessSpecifier)
    {
        glGenBuffers(1, &m_id);
    }

    ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer&& p_other) noexcept
        : m_id(p_other.m_id), m_bindIndex(p_other.m_bindIndex), m_accessSpecifier(p_other.m_accessSpecifier)
    {
        p_other.m_id = 0;
    }

    ShaderStorageBuffer::~ShaderStorageBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    ShaderStorageBuffer& ShaderStorageBuffer::operator=(ShaderStorageBuffer&& p_other) noexcept
    {
        if (this == &p_other)
            return *this;

        glDeleteBuffers(1, &m_id);

        m_id              = p_other.m_id;
        m_bindIndex       = p_other.m_bindIndex;
        m_accessSpecifier = p_other.m_accessSpecifier;

        p_other.m_id        = 0;
        p_other.m_bindIndex = 0;

        return *this;
    }

    void ShaderStorageBuffer::setBindIndex(const uint32_t bindIndex)
    {
        m_bindIndex = bindIndex;
    }

    void ShaderStorageBuffer::bind(const uint32_t bindIndex)
    {
        setBindIndex(bindIndex);
        bind();
    }

    void ShaderStorageBuffer::bind() const
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindIndex, m_id);
    }

    void ShaderStorageBuffer::unbind() const
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindIndex, 0);
    }

    void ShaderStorageBuffer::setRawData(const void* data, const size_t size) const
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
        glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(size), data, ToGLEnum(m_accessSpecifier));
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void ShaderStorageBuffer::setRawSubData(const void* data, const size_t size, const intptr_t offset) const
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, static_cast<GLsizeiptr>(size), data);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}

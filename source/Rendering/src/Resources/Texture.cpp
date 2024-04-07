#include "Resources/Texture.h"

#include "Debug/Assertion.h"
#include "Debug/Log.h"
#include "Vector/Vector4.h"

#include <glad/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) ASSERT(x)
#include <stb_image.h>

#define TEXTURE_MAX_SLOT GL_TEXTURE31 - GL_TEXTURE0

namespace LibGL::Rendering::Resources
{
    REGISTER_RESOURCE_TYPE(Texture);

    Texture::Texture(const std::filesystem::path& fileName)
    {
        ASSERT(load(fileName.string()));
    }

    Texture::Texture(const int width, const int height, const int channels)
        : m_width(width), m_height(height), m_channels(channels)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, getGLFormat(), GL_FLOAT, nullptr);
    }

    Texture::Texture(const int width, const int height, ETextureFormat format)
        : m_width(width), m_height(height)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        const GLenum texFormat = static_cast<GLenum>(format);
        glTexImage2D(GL_TEXTURE_2D, 0, texFormat, m_width, m_height, 0, texFormat, GL_FLOAT, nullptr);

        switch (format)
        {
        case ETextureFormat::RED:
            m_channels = 1;
            break;
        case ETextureFormat::RG:
            m_channels = 2;
            break;
        case ETextureFormat::RGB:
        case ETextureFormat::BGR:
            m_channels = 3;
            break;
        case ETextureFormat::RGBA:
        case ETextureFormat::BGRA:
            m_channels = 4;
            break;
        default:
            m_channels = 0;
            break;
        }
    }

    Texture::Texture(const Texture& other)
        : IResource(other), m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels)
    {
        if (other.m_data != nullptr)
            m_data = stbi_load_from_memory(other.m_data, other.m_width * other.m_height * other.m_channels, &m_width, &m_height,
                &m_channels, 0);
        else
            m_data = nullptr;

        if (other.m_id != 0)
        {
            glGenTextures(1, &m_id);
            glBindTexture(GL_TEXTURE_2D, m_id);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, getGLFormat(), GL_FLOAT, nullptr);

            glCopyImageSubData(other.m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
                m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
                m_width, m_height, 1);
        }
        else
        {
            m_id = 0;
        }
    }

    Texture::Texture(Texture&& other) noexcept
        : m_data(other.m_data), m_id(other.m_id), m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels)
    {
        other.m_id   = 0;
        other.m_data = nullptr;
    }

    Texture::~Texture()
    {
        if (m_data != nullptr)
            stbi_image_free(m_data);

        if (m_id != 0)
            glDeleteTextures(1, &m_id);
    }

    Texture& Texture::operator=(const Texture& other)
    {
        if (&other == this)
            return *this;

        m_width    = other.m_width;
        m_height   = other.m_height;
        m_channels = other.m_channels;

        if (m_id != 0)
            glDeleteTextures(1, &m_id);

        if (m_data != nullptr)
            stbi_image_free(m_data);

        if (other.m_data != nullptr)
            m_data = stbi_load_from_memory(other.m_data, other.m_width * other.m_height, &m_width, &m_height, &m_channels, 0);
        else
            m_data = nullptr;

        if (other.m_id != 0)
        {
            glGenTextures(1, &m_id);
            glBindTexture(GL_TEXTURE_2D, m_id);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, getGLFormat(), GL_FLOAT, nullptr);

            glCopyImageSubData(other.m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
                m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
                m_width, m_height, 1);
        }
        else
        {
            m_id = 0;
        }

        return *this;
    }

    Texture& Texture::operator=(Texture&& other) noexcept
    {
        if (&other == this)
            return *this;

        glDeleteTextures(1, &m_id);

        if (m_data != nullptr)
            stbi_image_free(m_data);

        m_id       = other.m_id;
        m_width    = other.m_width;
        m_height   = other.m_height;
        m_channels = other.m_channels;
        m_data     = other.m_data;

        other.m_id   = 0;
        other.m_data = nullptr;

        return *this;
    }

    Texture& Texture::getDefault()
    {
        static Texture texture;

        if (texture.m_id == 0)
        {
            texture.m_width    = 1;
            texture.m_height   = 1;
            texture.m_channels = 4;

            glGenTextures(1, &texture.m_id);
            glBindTexture(GL_TEXTURE_2D, texture.m_id);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.m_width, texture.m_height,
                0, texture.getGLFormat(), GL_FLOAT, LibMath::Vector4(1).getArray());

            texture.setWrapModeU(ETextureWrapMode::REPEAT);
            texture.setWrapModeV(ETextureWrapMode::REPEAT);

            texture.setMinFilter(ETextureFilter::NEAREST);
            texture.setMagFilter(ETextureFilter::NEAREST);
        }

        return texture;
    }

    bool Texture::load(const char* fileName)
    {
        if (m_data != nullptr)
            stbi_image_free(m_data);

        stbi_set_flip_vertically_on_load(true);
        m_data = stbi_load(fileName, &m_width, &m_height, &m_channels, 0);

        if (m_data == nullptr)
        {
            DEBUG_LOG("Unable to load texture from path \"%s\"\n", fileName);
            return false;
        }

        return true;
    }

    bool Texture::init()
    {
        if (m_data == nullptr)
        {
            DEBUG_LOG("Unable to initialize texture - no pixels\n");
            return false;
        }

        if (m_id == 0)
        {
            glGenTextures(1, &m_id);

            if (m_id == 0)
            {
                DEBUG_LOG("Unable to generate texture id.\n");
                return false;
            }
        }

        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, getGLFormat(),
            GL_UNSIGNED_BYTE, m_data);

        glGenerateMipmap(GL_TEXTURE_2D);
        setMinFilter(ETextureFilter::LINEAR_MIPMAP_LINEAR);

        stbi_image_free(m_data);
        m_data = nullptr;

        return true;
    }

    uint32_t Texture::getId() const
    {
        return m_id;
    }

    void Texture::bind(const uint8_t slot) const
    {
        ASSERT(slot <= TEXTURE_MAX_SLOT, "Texture slot %d is out of bounds. Max value: %d", slot, TEXTURE_MAX_SLOT);

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_wrapModeU));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_wrapModeV));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_minFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_magFilter));
    }

    void Texture::unbind(const uint8_t slot)
    {
        ASSERT(slot <= TEXTURE_MAX_SLOT, "Texture slot %d is out of bounds. Max value: %d", slot, TEXTURE_MAX_SLOT);

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::setWrapModeU(const ETextureWrapMode wrapMode)
    {
        m_wrapModeU = wrapMode;
    }

    void Texture::setWrapModeV(const ETextureWrapMode wrapMode)
    {
        m_wrapModeV = wrapMode;
    }

    void Texture::setMinFilter(const ETextureFilter textureFilter)
    {
        m_minFilter = textureFilter;
    }

    void Texture::setMagFilter(const ETextureFilter textureFilter)
    {
        m_magFilter = textureFilter;
    }

    int Texture::getWidth() const
    {
        return m_width;
    }

    int Texture::getHeight() const
    {
        return m_height;
    }

    uint32_t Texture::getGLFormat() const
    {
        switch (m_channels)
        {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            ASSERT(false, "Invalid channels count. Accepted 1, 2, 3 or 4 but received \"%d\".\n", m_channels);
            return GL_INVALID_ENUM;
        }
    }
}

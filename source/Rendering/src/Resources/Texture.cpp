#include "Debug/Log.h"
#include "Debug/Assertion.h"
#include "Resources/Texture.h"

#include <glad/glad.h>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) ASSERT(x)
#include <stb_image.h>

LibGL::Resources::Texture::Texture(const std::filesystem::path& fileName)
{
	if (!Texture::loadFromFile(fileName.string()))
		throw std::runtime_error("Unable to load texture from path \""
			+ fileName.string() + "\"");
}

LibGL::Resources::Texture::Texture(const Texture& other) :
	m_id(other.m_id), m_width(other.m_width), m_height(other.m_height),
	m_channels(other.m_channels)
{
}

LibGL::Resources::Texture::Texture(Texture&& other) noexcept :
	m_id(other.m_id), m_width(other.m_width), m_height(other.m_height),
	m_channels(other.m_channels)
{
	other.m_id = 0;
}

LibGL::Resources::Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

LibGL::Resources::Texture& LibGL::Resources::Texture::operator=(const Texture& other)
{
	if (&other == this)
		return *this;

	m_id = other.m_id;
	m_width = other.m_width;
	m_height = other.m_height;
	m_channels = other.m_channels;

	return *this;
}

LibGL::Resources::Texture& LibGL::Resources::Texture::operator=(Texture&& other) noexcept
{
	if (&other == this)
		return *this;

	m_id = other.m_id;
	m_width = other.m_width;
	m_height = other.m_height;
	m_channels = other.m_channels;

	other.m_id = 0;

	return *this;
}

bool LibGL::Resources::Texture::loadFromFile(const std::string& fileName)
{
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* data = stbi_load(fileName.c_str(), &m_width, &m_height, &m_channels, 0);

	if (data == nullptr)
	{
		DEBUG_LOG("Unable to load texture from path \"%s\"", fileName.c_str());
		return false;
	}

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, getGLFormat(),
		GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	return true;
}

void LibGL::Resources::Texture::use() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void LibGL::Resources::Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t LibGL::Resources::Texture::getGLFormat() const
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
		DEBUG_LOG("Invalid channels count. Accepted 1, 2, 3 or 4 but received \"%d\".", m_channels);
		throw std::out_of_range("Invalid channels count - Accepted 1, 2, 3 or 4. "
			"Received \"" + std::to_string(m_channels) + "\".");
	}
}

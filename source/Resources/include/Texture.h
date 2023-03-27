#pragma once
#include <filesystem>
#include <string>

#include "IResource.h"
#include "glad/glad.h"

namespace My::Resources
{
	class Texture : public IResource
	{
	public:
		Texture() = default;
		explicit Texture(const std::filesystem::path& fileName);
		Texture(const Texture& other);
		Texture(Texture&& other) noexcept;
		~Texture() override;

		Texture& operator=(const Texture& other);
		Texture& operator=(Texture&& other) noexcept;

		bool loadFromFile(const std::string& fileName) override;
		void use() const;

	private:
		GLuint		m_id = 0;
		int			m_width = 0;
		int			m_height = 0;
		int			m_channels = 0;

		GLenum getGLFormat() const;
	};
}

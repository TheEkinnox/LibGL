#pragma once
#include <filesystem>
#include <string>
#include <Resources/IResource.h>

namespace LibGL::Resources
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

		/**
		 * \brief Tries to load a texture from the given file
		 * \param fileName The texture's file path
		 * \return True if the texture was successfully loaded. False otherwise.
		 */
		bool loadFromFile(const std::string& fileName) override;

		/**
		 * \brief Binds the texture to the current OpenGL context
		 */
		void use() const;

		/**
		 * \brief Unbinds the current texture from the OpenGL context
		 */
		static void unbind();

	private:
		uint32_t	m_id = 0;
		int			m_width = 0;
		int			m_height = 0;
		int			m_channels = 0;

		/**
		 * \brief Gets the color format corresponding to the texture's
		 * channel count.
		 * \return The GLenum corresponding to the current channels count
		 */
		uint32_t getGLFormat() const;
	};
}

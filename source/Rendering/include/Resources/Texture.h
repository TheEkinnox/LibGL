#pragma once
#include <filesystem>
#include <string>
#include <Resources/IResource.h>

#include "Enums/ETextureFilter.h"
#include "Enums/ETextureWrapMode.h"

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
		 * \brief Gets or creates a white 1x1 texture
		 * \return A reference to the default texture
		 */
		static Texture& getDefault();

		/**
		 * \brief Tries to load a texture from the given file
		 * \param fileName The texture's file path
		 * \return True if the texture was successfully loaded. False otherwise.
		 */
		bool loadFromFile(const std::string& fileName) override;

		/**
		 * \brief Binds the texture to the current OpenGL context
		 * \param slot The slot the texture is bound to
		 */
		void bind(uint8_t slot = 0) const;

		/**
		 * \brief Unbinds the current texture from the OpenGL context
		 * \param slot The slot the texture is bound to
		 */
		static void unbind(uint8_t slot = 0);

		/**
		 * \brief Sets the texture's horizontal wrap mode
		 * \param wrapMode The texture's new horizontal wrap mode
		 */
		void setWrapModeU(Rendering::ETextureWrapMode wrapMode);

		/**
		 * \brief Sets the texture's vertical wrap mode
		 * \param wrapMode The texture's new vertical wrap mode
		 */
		void setWrapModeV(Rendering::ETextureWrapMode wrapMode);

		/**
		 * \brief Set the texture's minification filter
		 * \param textureFilter The texture's new minification filter
		 */
		void setMinFilter(Rendering::ETextureFilter textureFilter);

		/**
		 * \brief Set the texture's magnification filter
		 * \param textureFilter The texture's new magnification filter
		 */
		void setMagFilter(Rendering::ETextureFilter textureFilter);

	private:
		uint32_t					m_id = 0;
		int							m_width = 0;
		int							m_height = 0;
		int							m_channels = 0;
		Rendering::ETextureFilter	m_minFilter = Rendering::ETextureFilter::LINEAR;
		Rendering::ETextureFilter	m_magFilter = Rendering::ETextureFilter::LINEAR;
		Rendering::ETextureWrapMode	m_wrapModeU = Rendering::ETextureWrapMode::REPEAT;
		Rendering::ETextureWrapMode	m_wrapModeV = Rendering::ETextureWrapMode::REPEAT;

		/**
		 * \brief Gets the color format corresponding to the texture's
		 * channel count.
		 * \return The GLenum corresponding to the current channels count
		 */
		uint32_t getGLFormat() const;
	};
}

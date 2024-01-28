#pragma once
#include "Enums/EFrameBufferAttachment.h"
#include "Enums/ETextureFilter.h"
#include "Enums/ETextureFormat.h"
#include "Enums/ETextureWrapMode.h"

#include "Resources/IResource.h"

#include <filesystem>

namespace LibGL::Rendering::Resources
{
    class Texture final : public LibGL::Resources::IResource
    {
    public:
        using IResource::load;

        Texture() = default;
        explicit Texture(const std::filesystem::path& fileName);
        Texture(int width, int height, int channels);
        Texture(int width, int height, ETextureFormat format);
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
         * \brief Loads a texture from the given file
         * \param fileName The texture's file path
         * \return True if the texture was successfully loaded. False otherwise.
         */
        bool load(const char* fileName) override;

        /**
         * \brief Initializes the texture
         * \return True if the texture was successfully initialized. False otherwise.
         */
        bool init() override;

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
        void setWrapModeU(ETextureWrapMode wrapMode);

        /**
         * \brief Sets the texture's vertical wrap mode
         * \param wrapMode The texture's new vertical wrap mode
         */
        void setWrapModeV(ETextureWrapMode wrapMode);

        /**
         * \brief Set the texture's minification filter
         * \param textureFilter The texture's new minification filter
         */
        void setMinFilter(ETextureFilter textureFilter);

        /**
         * \brief Set the texture's magnification filter
         * \param textureFilter The texture's new magnification filter
         */
        void setMagFilter(ETextureFilter textureFilter);

        /**
         * \brief Attaches the texture to the current frame buffer
         */
        void attachToFrameBuffer(EFrameBufferAttachment attachmentMode) const;

        /**
         * \brief Provides read access to the texture's width
         * \return The texture's width
         */
        int getWidth() const;

        /**
         * \brief Provides read access to the texture's height
         * \return The texture's height
         */
        int getHeight() const;

    private:
        unsigned char*   m_data      = nullptr;
        uint32_t         m_id        = 0;
        int              m_width     = 0;
        int              m_height    = 0;
        int              m_channels  = 0;
        ETextureFilter   m_minFilter = ETextureFilter::LINEAR;
        ETextureFilter   m_magFilter = ETextureFilter::LINEAR;
        ETextureWrapMode m_wrapModeU = ETextureWrapMode::REPEAT;
        ETextureWrapMode m_wrapModeV = ETextureWrapMode::REPEAT;

        /**
         * \brief Gets the color format corresponding to the texture's
         * channel count.
         * \return The GLenum corresponding to the current channels count
         */
        uint32_t getGLFormat() const;
    };
}

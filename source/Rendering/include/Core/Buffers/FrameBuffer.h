#pragma once
#include "Resources/Texture.h"

#include <cstdint>

namespace LibGL::Rendering
{
    class FrameBuffer final
    {
    public:
        FrameBuffer();
        FrameBuffer(const FrameBuffer& other) = delete;
        FrameBuffer(FrameBuffer&& other) noexcept;
        ~FrameBuffer();

        FrameBuffer& operator=(const FrameBuffer& other) = delete;
        FrameBuffer& operator=(FrameBuffer&& other) noexcept;

        /**
         * \brief Binds the frame buffer to the current context
         */
        void bind() const;

        /**
         * \brief Unbinds the current frame buffer
         */
        void unbind() const;

        /**
         * \brief Attaches the given texture to the frame buffer
         * \param texture The texture to attach
         * \param attachmentMode The texture's target attachment
         */
        void attach(const Resources::Texture& texture, EFrameBufferAttachment attachmentMode) const;

        /**
         * \brief Detaches the attachment with the given mode from the frame buffer
         * \param attachmentMode The attachment to detach
         */
        void detach(EFrameBufferAttachment attachmentMode) const;

    private:
        uint32_t m_id = 0;
    };
}

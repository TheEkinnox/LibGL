#pragma once
#include "Core/Buffers/Buffer.h"

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
        static void unbind();

    private:
        uint32_t m_bufferIndex = 0;
    };
}

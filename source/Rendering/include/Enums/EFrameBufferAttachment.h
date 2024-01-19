#pragma once

namespace LibGL::Rendering
{
    /**
     * \brief OpenGL frame buffer attachment enum wrapper
     */
    enum class EFrameBufferAttachment
    {
        COLOR_0       = 0x8CE0,
        COLOR_MAX     = 0x8CFF,
        DEPTH         = 0x8D00,
        STENCIL       = 0x8D20,
        DEPTH_STENCIL = 0x821A
    };
}

#pragma once

namespace LibGL::Rendering
{
    /**
    * OpenGL texture wrap mode enum wrapper
    */
    enum class ETextureWrapMode
    {
        REPEAT          = 0x2901,
        MIRRORED_REPEAT = 0x8370,
        CLAMP_TO_EDGE   = 0x812F,
        CLAMP_TO_BORDER = 0x812D
    };
}

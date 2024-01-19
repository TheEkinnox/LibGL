#pragma once

namespace LibGL::Rendering
{
    /**
     * \brief OpenGL texture format enum wrapper
     */
    enum class ETextureFormat
    {
        RED             = 0x1903,
        RG              = 0x8227,
        RGB             = 0x1907,
        RGBA            = 0x1908,
        BGR             = 0x80E0,
        BGRA            = 0x80E1,
        STENCIL_INDEX   = 0x1901,
        DEPTH_COMPONENT = 0x1902,
        DEPTH_STENCIL   = 0x84F9
    };
}

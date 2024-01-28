#pragma once

namespace LibGL::Rendering
{
    /**
     * \brief OpenGL cull-face enum wrapper
     */
    enum class ECullFace
    {
        FRONT          = 0x0404,
        BACK           = 0x0405,
        FRONT_AND_BACK = 0x0408
    };
}

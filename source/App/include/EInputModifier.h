#pragma once

namespace LibGL::Application
{
#undef MOD_SHIFT
#undef MOD_CONTROL
#undef MOD_ALT
    enum class EInputModifier
    {
        MOD_SHIFT     = 1 << 0,
        MOD_CONTROL   = 1 << 1,
        MOD_ALT       = 1 << 2,
        MOD_SUPER     = 1 << 3,
        MOD_CAPS_LOCK = 1 << 4,
        MOD_NUM_LOCK  = 1 << 5
    };
}

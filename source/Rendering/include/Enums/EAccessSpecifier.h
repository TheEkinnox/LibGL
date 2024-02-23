#pragma once
#include <cstdint>

namespace LibGL::Rendering
{
    /**
     * \brief Supported buffer access specifiers
     */
    enum class EAccessSpecifier : uint8_t
    {
        STREAM_DRAW,
        STREAM_READ,
        STREAM_COPY,
        DYNAMIC_DRAW,
        DYNAMIC_READ,
        DYNAMIC_COPY,
        STATIC_DRAW,
        STATIC_READ,
        STATIC_COPY
    };
}

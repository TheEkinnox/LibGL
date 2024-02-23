#pragma once
#include "Core/Buffers/ShaderStorageBuffer.h"

namespace LibGL::Rendering
{
    template <typename T>
    void ShaderStorageBuffer::sendBlocks(const T* data, const size_t count) const
    {
        sendBlocks(static_cast<const void*>(data), count * sizeof(T));
    }
}

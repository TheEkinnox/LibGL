#pragma once
#include "Core/Buffers/ShaderStorageBuffer.h"

namespace LibGL::Rendering
{
    template <typename T>
    void ShaderStorageBuffer::setData(const T* data, const size_t count) const
    {
        setRawData(static_cast<const void*>(data), count * sizeof(T));
    }

    template <typename T>
    void ShaderStorageBuffer::setSubData(const T* data, const size_t count, const ptrdiff_t offset) const
    {
        setRawSubData(static_cast<const void*>(data), count * sizeof(T), offset);
    }
}

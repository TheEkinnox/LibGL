#pragma once
#include "Core/Buffers/ShaderStorageBuffer.inl"

namespace LibGL::Rendering
{
	template <typename T>
	void ShaderStorageBuffer::sendBlocks(T* data, const size_t count) const
	{
		sendBlocks(static_cast<const void*>(data), count * sizeof(T));
	}
}

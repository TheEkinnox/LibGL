#pragma once
#include <vector>

#include "Core/Buffers/Buffer.h"

namespace LibGL::Rendering
{
	class IndexBuffer final : public Buffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(const uint32_t* indices, intptr_t indexCount);
		explicit IndexBuffer(const std::vector<uint32_t>& indices);

		/**
		 * \brief Binds the index buffer object to the current context
		 */
		void bind() const override;

		/**
		 * \brief Unbinds the index buffer from the current context
		 */
		static void unbind();
	};
}

#pragma once
#include <vector>

#include "Core/Buffers/Buffer.h"
#include "Resources/Vertex.h"

namespace LibGL::Rendering
{
	class VertexBuffer final : public Buffer
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(const Resources::Vertex* vertices, intptr_t verticesCount);
		explicit VertexBuffer(const std::vector<Resources::Vertex>& vertices);

		/**
		 * \brief Binds the vertex buffer to the current context
		 */
		void bind() const override;

		/**
		 * \brief Unbinds the vertex buffer from the current context
		 */
		static void unbind();
	};
}

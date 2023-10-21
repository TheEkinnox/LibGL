#pragma once

#include "Core/Buffers/VertexBuffer.h"
#include "Core/Buffers/IndexBuffer.h"

namespace LibGL::Rendering
{
	class VertexAttributes
	{
	public:
		VertexAttributes() = default;
		explicit VertexAttributes(const VertexBuffer& vbo, const IndexBuffer& ebo);

		VertexAttributes(const VertexAttributes& other) = delete;
		VertexAttributes(VertexAttributes&& other) noexcept;
		~VertexAttributes();

		VertexAttributes& operator=(const VertexAttributes& other) = delete;
		VertexAttributes& operator=(VertexAttributes&& other) noexcept;

		/**
		 * \brief Binds the vertex attributes object to the current context
		 */
		void bind() const;

		/**
		 * \brief Unbinds the vertex attributes object from the current context
		 */
		static void unbind();

	private:
		uint32_t	m_vao = 0;
	};
}
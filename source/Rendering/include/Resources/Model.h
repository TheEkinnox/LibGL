#pragma once
#include <string>
#include <vector>

#include "IResource.h"
#include "Vertex.h"

namespace LibGL::Resources
{
	class Model final : public IResource
	{
		class Buffer
		{
		public:
			Buffer(const Buffer& other) = delete;
			Buffer(Buffer&& other) noexcept;
			virtual ~Buffer();

			Buffer& operator=(const Buffer& other) = delete;
			Buffer& operator=(Buffer&& other) noexcept;

			/**
			 * \brief Binds the buffer to the current context
			 */
			virtual void bind() const = 0;

		protected:
			Buffer() = default;

			uint32_t	m_bufferIndex = 0;
		};

	public:
		class VertexBuffer final : public Buffer
		{
		public:
			VertexBuffer() = default;
			VertexBuffer(const Vertex* vertices, intptr_t verticesCount);
			explicit VertexBuffer(const std::vector<Vertex>& vertices);

			/**
			 * \brief Binds the vertex buffer to the current context
			 */
			void bind() const override;

			/**
			 * \brief Unbinds the vertex buffer from the current context
			 */
			static void unbind();
		};

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

		Model() = default;
		Model(const Model& other);
		Model(Model&& other) noexcept;
		~Model() override = default;

		Model&	operator=(const Model& other);
		Model&	operator=(Model&& other) noexcept;

		/**
		 * \brief Loads a wavefront object model from the given file
		 * \param fileName The path to the .obj model to load
		 * \return True if the model could be loaded. False otherwise.
		 */
		bool loadFromFile(const std::string& fileName) override;

		/**
		 * \brief Draws the model
		 */
		void draw() const;

	private:
		std::vector<Vertex>		m_vertices;
		std::vector<uint32_t>	m_indices;
		VertexBuffer			m_vbo;
		IndexBuffer				m_ebo;
		VertexAttributes		m_vao;
	};
}

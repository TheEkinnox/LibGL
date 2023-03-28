#pragma once
#include <string>
#include <vector>

#include "IResource.h"
#include "Vertex.h"

namespace My::Resources
{
	class Model final : public IResource
	{
	public:
		class Buffer
		{
		public:
			Buffer() = default;
			Buffer(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
			Buffer(const Buffer& other) = delete;
			Buffer(Buffer&& other) noexcept;
			~Buffer();

			Buffer& operator=(const Buffer& other) = delete;
			Buffer& operator=(Buffer&& other) noexcept;

			/**
			 * \brief Binds the vertex buffer object and element buffer object
			 */
			void bind() const;

		private:
			uint32_t	m_vbo = 0;
			uint32_t	m_ebo = 0;
		};

		class VertexAttributes
		{
		public:
			VertexAttributes() = default;
			explicit VertexAttributes(const Buffer& buffer);

			VertexAttributes(const VertexAttributes& other) = delete;
			VertexAttributes(VertexAttributes&& other) noexcept;
			~VertexAttributes();

			VertexAttributes& operator=(const VertexAttributes& other) = delete;
			VertexAttributes& operator=(VertexAttributes&& other) noexcept;

			/**
			 * \brief Binds the vertex attributes object
			 */
			void bind() const;

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
		VertexAttributes		m_vertexAttributes;
		Buffer					m_buffer;
	};
}

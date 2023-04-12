#pragma once
#include <string>
#include <vector>
#include <Resources/IResource.h>

#include "Vertex.h"
#include "Core/Buffers/IndexBuffer.h"
#include "Core/Buffers/VertexBuffer.h"

namespace LibGL::Resources
{
	class Model final : public IResource
	{
	public:
		class VertexAttributes
		{
		public:
			VertexAttributes() = default;
			explicit VertexAttributes(const Rendering::VertexBuffer& vbo, const Rendering::IndexBuffer& ebo);

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
		Rendering::VertexBuffer	m_vbo;
		Rendering::IndexBuffer	m_ebo;
		VertexAttributes		m_vao;
	};
}

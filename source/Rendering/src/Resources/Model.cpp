#include "Resources/Model.h"

#include "Debug/Assertion.h"
#include "Debug/Log.h"

#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include "Utility/utility.h"

using namespace LibMath;

namespace LibGL::Resources
{
	Model::VertexBuffer::VertexBuffer(const Vertex* vertices, const intptr_t verticesCount)
	{
		glGenBuffers(1, &m_bufferIndex);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferIndex);
		glBufferData(GL_ARRAY_BUFFER, verticesCount * static_cast<GLsizeiptr>(sizeof(Vertex)),
			vertices, GL_STATIC_DRAW);
	}

	Model::VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
	{
		const Vertex* verticesArray = vertices.data();

		glGenBuffers(1, &m_bufferIndex);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferIndex);
		glBufferData(GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(vertices.size()) * static_cast<GLsizeiptr>(sizeof(Vertex)),
			verticesArray, GL_STATIC_DRAW);
	}

	Model::IndexBuffer::IndexBuffer(const uint32_t* indices, const intptr_t indexCount)
	{
		glGenBuffers(1, &m_bufferIndex);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferIndex);
		glBufferData(GL_ARRAY_BUFFER, indexCount * static_cast<GLsizeiptr>(sizeof(Vertex)),
			indices, GL_STATIC_DRAW);
	}

	Model::IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices)
	{
		const uint32_t* idsArray = indices.data();

		glGenBuffers(1, &m_bufferIndex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferIndex);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(indices.size()) * static_cast<GLsizeiptr>(sizeof(uint32_t)),
			idsArray, GL_STATIC_DRAW);
	}

	Model::Buffer::Buffer(Buffer&& other) noexcept
		: m_bufferIndex(other.m_bufferIndex)
	{
		other.m_bufferIndex = 0;
	}

	Model::Buffer::~Buffer()
	{
		glDeleteBuffers(1, &m_bufferIndex);
	}

	Model::Buffer& Model::Buffer::operator=(Buffer&& other) noexcept
	{
		if (&other == this)
			return *this;

		glDeleteBuffers(1, &m_bufferIndex);

		m_bufferIndex = other.m_bufferIndex;

		other.m_bufferIndex = 0;

		return *this;
	}

	void Model::VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferIndex);
	}

	void Model::VertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Model::IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferIndex);
	}

	void Model::IndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	Model::VertexAttributes::VertexAttributes(const VertexBuffer& vbo, const IndexBuffer& ebo)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		vbo.bind();
		ebo.bind();

		constexpr auto stride = sizeof(Vertex);

		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);

		// normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(sizeof(Vector3)));

		// texture coordinates attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(2 * sizeof(Vector3)));
	}

	Model::VertexAttributes::VertexAttributes(VertexAttributes&& other) noexcept
		: m_vao(other.m_vao)
	{
		other.m_vao = 0;
	}

	Model::VertexAttributes::~VertexAttributes()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	Model::VertexAttributes& Model::VertexAttributes::operator=(
		VertexAttributes&& other) noexcept
	{
		if (&other == this)
			return *this;

		glDeleteVertexArrays(1, &m_vao);

		m_vao = other.m_vao;

		other.m_vao = 0;

		return *this;
	}

	void Model::VertexAttributes::bind() const
	{
		glBindVertexArray(m_vao);
	}

	void Model::VertexAttributes::unbind()
	{
		glBindVertexArray(0);
	}

	Model::Model(const Model& other)
		: m_vertices(other.m_vertices), m_indices(other.m_indices),
		m_vbo(m_vertices), m_ebo(m_indices)
	{
	}

	Model::Model(Model&& other) noexcept
		: m_vertices(std::move(other.m_vertices)), m_indices(std::move(other.m_indices)),
		m_vbo(m_vertices), m_ebo(m_indices)
	{
	}

	Model& Model::operator=(const Model& other)
	{
		if (&other == this)
			return *this;

		m_indices = other.m_indices;
		m_vertices = other.m_vertices;

		return *this;
	}

	Model& Model::operator=(Model&& other) noexcept
	{
		if (&other == this)
			return *this;

		m_indices = std::move(other.m_indices);
		m_vertices = std::move(other.m_vertices);

		return *this;
	}

	bool Model::loadFromFile(const std::string& fileName)
	{
		std::ifstream fs(fileName);

		if (!fs.is_open())
		{
			DEBUG_LOG("Unable to open file at path \"%s\"\n", fileName.c_str());
			return false;
		}

		std::vector<Vector3>	positions, normals;
		std::vector<Vector2>	uvs;

		std::string line;

		while (std::getline(fs, line))
		{
			std::string token(line.substr(0, 2));

			if (token == "v ")
			{
				std::istringstream posString(line.substr(2));
				Vector3 pos;

				posString >> pos.m_x;
				posString >> pos.m_y;
				posString >> pos.m_z;

				positions.push_back(pos);
			}
			else if (token == "vt")
			{
				std::istringstream uvString(line.substr(3));
				Vector2 uv;

				uvString >> uv.m_x;
				uvString >> uv.m_y;

				uvs.push_back(uv);
			}
			else if (token == "vn")
			{
				std::istringstream normalString(line.substr(3));
				Vector3 normal;

				normalString >> normal.m_x;
				normalString >> normal.m_y;
				normalString >> normal.m_z;

				normals.push_back(normal);
			}
			else if (token == "f ")
			{
				line = line.substr(token.size());
				const auto faceStrings = Utility::splitString(line, " ", true);

				ASSERT(faceStrings.size() == 3);

				for (const auto& faceStr : faceStrings)
				{
					size_t posIdx = 0, uvIdx = 0, normalIdx = 0;

					const auto faceData = Utility::splitString(faceStr, "/", true);

					if (!faceData[0].empty())
					{
						if (std::stol(faceData[0]) < 0)
							posIdx = positions.size() + 1 + std::stol(faceData[0]);
						else
							posIdx = std::stoul(faceData[0]);
					}

					if (!faceData[1].empty())
					{
						if (std::stol(faceData[1]) < 0)
							uvIdx = uvs.size() + 1 + std::stol(faceData[1]);
						else
							uvIdx = std::stoul(faceData[1]);
					}

					if (!faceData[2].empty())
					{
						if (std::stol(faceData[2]) < 0)
							normalIdx = normals.size() + 1 + std::stol(faceData[2]);
						else
							normalIdx = std::stoul(faceData[2]);
					}

					const Vertex vertex
					{
						posIdx > 0 ? positions[posIdx - 1] : Vector3(),
						normalIdx > 0 ? normals[normalIdx - 1] : Vector3(),
						uvIdx > 0 ? uvs[uvIdx - 1] : Vector2()
					};

					uint32_t	vertexIdx;
					bool		isDuplicate = false;
					
					//vertexIdx = static_cast<uint32_t>(m_vertices.size());
					for (vertexIdx = 0; vertexIdx < m_vertices.size(); ++vertexIdx)
					{
						if (m_vertices[vertexIdx] == vertex)
						{
							isDuplicate = true;
							break;
						}
					}

					if (!isDuplicate)
						m_vertices.push_back(vertex);

					m_indices.push_back(vertexIdx);
				}
			}
		}

		m_vbo = VertexBuffer(m_vertices);
		m_ebo = IndexBuffer(m_indices);
		m_vao = VertexAttributes(m_vbo, m_ebo);

		return true;
	}

	void Model::draw() const
	{
		m_vao.bind();
		m_vbo.bind();
		m_ebo.bind();

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
			GL_UNSIGNED_INT, nullptr);
	}
}

#include "Model.h"

#include "Debug/Assertion.h"
#include "Debug/Log.h"

#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include "utility.h"

using namespace LibMath;

namespace My::Resources
{
	Model::Buffer::Buffer(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		const Vertex* vertsArray = vertices.data();

		const uint32_t* idsArray = indices.data();

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(vertices.size()) * sizeof(Vertex),
			vertsArray, GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(indices.size()) * sizeof(uint32_t),
			idsArray, GL_STATIC_DRAW);
	}

	Model::Buffer::Buffer(Buffer&& other) noexcept
		: m_vbo(other.m_vbo), m_ebo(other.m_ebo)
	{
		other.m_vbo = other.m_ebo = 0;
	}

	Model::Buffer::~Buffer()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}

	Model::Buffer& Model::Buffer::operator=(Buffer&& other) noexcept
	{
		if (&other == this)
			return *this;

		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);

		m_vbo = other.m_vbo;
		m_ebo = other.m_ebo;

		other.m_vbo = other.m_ebo = 0;

		return *this;
	}

	void Model::Buffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	}

	Model::VertexAttributes::VertexAttributes(const Buffer& buffer)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		buffer.bind();

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

	Model::Model(const Model& other)
		: m_vertices(other.m_vertices), m_indices(other.m_indices),
		m_buffer(m_vertices, m_indices)
	{
	}

	Model::Model(Model&& other) noexcept
		: m_vertices(std::move(other.m_vertices)), m_indices(std::move(other.m_indices)),
		m_buffer(m_vertices, m_indices)
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
				const auto faceStrings = splitString(line.substr(token.size()), " ", true);

				ASSERT(faceStrings.size() == 3);

				for (const auto& faceStr : faceStrings)
				{
					size_t posIdx = 0, uvIdx = 0, normalIdx = 0;

					const auto faceData = splitString(faceStr, "/", true);

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

		m_buffer = Buffer(m_vertices, m_indices);
		m_vertexAttributes = VertexAttributes(m_buffer);

		return true;
	}

	void Model::draw() const
	{
		m_vertexAttributes.bind();
		m_buffer.bind();

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
			GL_UNSIGNED_INT, nullptr);
	}
}

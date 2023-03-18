#include "Model.h"

#include <fstream>
#include <sstream>

#include "Debug/Assertion.h"
#include "Debug/Log.h"

using namespace LibMath;

My::Resources::Model::Model(const Model& other)
	: m_vertices(other.m_vertices), m_indices(other.m_indices)
{
}

My::Resources::Model::Model(Model&& other) noexcept
	: m_vertices(std::move(other.m_vertices)), m_indices(std::move(other.m_indices))
{
}

My::Resources::Model& My::Resources::Model::operator=(const Model& other)
{
	if (&other == this)
		return *this;

	m_indices = other.m_indices;
	m_vertices = other.m_vertices;

	return *this;
}

My::Resources::Model& My::Resources::Model::operator=(Model&& other) noexcept
{
	if (&other == this)
		return *this;

	m_indices = std::move(other.m_indices);
	m_vertices = std::move(other.m_vertices);

	return *this;
}

bool My::Resources::Model::LoadFromFile(const std::string& fileName)
{
	std::ifstream fs(fileName);

	if (!fs.is_open())
	{
		DEBUG_LOG("Unable to open file at path \"%s\"\n", fileName.c_str());
		return false;
	}

	struct FaceIndex
	{
		size_t	m_pos;
		size_t	m_uv;
		size_t	m_normal;
	};

	std::vector<Vector3>	positions, normals;
	std::vector<Vector2>	uvs;

	std::string line;

	while(std::getline(fs, line))
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
			FaceIndex indices[3]{};

			ASSERT(sscanf_s(line.c_str(), "f %llu/%llu/%llu %llu/%llu/%llu %llu/%llu/%llu",
				&indices[0].m_pos, &indices[0].m_uv, &indices[0].m_normal,
				&indices[1].m_pos, &indices[1].m_uv, &indices[1].m_normal,
				&indices[2].m_pos, &indices[2].m_uv, &indices[2].m_normal) % 3 == 0);

			for (auto& [posIdx, uvIdx, normalIdx] : indices)
			{
				const Vertex vertex
				{
					positions[posIdx - 1],
					normals[normalIdx - 1],
					uvs[uvIdx - 1]
				};

				size_t	vertexIdx;
				bool	isDuplicate = false;

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

	return true;
}

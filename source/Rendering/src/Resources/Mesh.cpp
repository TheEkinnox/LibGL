#include "Resources/Mesh.h"

#include <sstream>

#include <Debug/Assertion.h>
#include <Debug/Log.h>

#include <glad/gl.h>

#include <Utility/utility.h>

#define IGNORE_DUPLICATES 1

using namespace LibMath;

namespace LibGL::Rendering::Resources
{
    REGISTER_RESOURCE_TYPE(Mesh);

    bool Mesh::load(const char* fileName)
    {
        std::vector<std::string> lines = LibGL::Utility::readFile(fileName);

        if (lines.empty())
            return false;

        m_vertices.clear();
        m_indices.clear();

        std::vector<Vector3> positions, normals;
        std::vector<Vector2> uvs;

        for (std::string& line : lines)
        {
            std::string token(line.substr(0, 2));

            if (token == "v ")
                positions.push_back(parseVector3(line.substr(2)));
            else if (token == "vt")
                uvs.push_back(parseVector2(line.substr(3)));
            else if (token == "vn")
                normals.push_back(parseVector3(line.substr(3)));
            else if (token == "f ")
                parseFace(line.substr(2), positions, normals, uvs);
        }

        return true;
    }

    bool Mesh::init()
    {
        m_vbo = VertexBuffer(m_vertices);
        m_ebo = IndexBuffer(m_indices);
        m_vao = VertexAttributes(m_vbo, m_ebo);

        return true;
    }

    void Mesh::draw() const
    {
        m_vao.bind();
        m_vbo.bind();
        m_ebo.bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
            GL_UNSIGNED_INT, nullptr);
    }

    Vector3 Mesh::parseVector3(const std::string& vec3Str)
    {
        std::istringstream vec3Stream(vec3Str);
        Vector3            vec3;

        vec3Stream >> vec3.m_x;
        vec3Stream >> vec3.m_y;
        vec3Stream >> vec3.m_z;

        return vec3;
    }

    Vector2 Mesh::parseVector2(const std::string& vec2Str)
    {
        std::istringstream vec2Stream(vec2Str);
        Vector2            vec2;

        vec2Stream >> vec2.m_x;
        vec2Stream >> vec2.m_y;

        return vec2;
    }

    std::vector<size_t> Mesh::getIndices(const size_t vertexCount)
    {
        ASSERT(vertexCount >= 3);

        std::vector<size_t> faceIndices;
        faceIndices.reserve((vertexCount - 2) * 3);

        for (uint32_t i = 1; i < vertexCount - 1; ++i)
        {
            faceIndices.push_back(0);
            faceIndices.push_back(i);
            faceIndices.push_back(i + 1);
        }

        return faceIndices;
    }

    Vertex Mesh::parseVertex(const std::string& str, const std::vector<Vector3>& positions, const std::vector<Vector3>& normals,
                              const std::vector<Vector2>& uvs)
    {
        size_t posIdx = 0, uvIdx = 0, normalIdx = 0;

        const auto faceData = Utility::splitString(str, "/", true);

        if (!faceData[0].empty())
        {
            if (std::stol(faceData[0]) < 0)
                posIdx = positions.size() + 1 + std::stol(faceData[0]);
            else
                posIdx = std::stoul(faceData[0]);
        }

        if (faceData.size() > 1 && !faceData[1].empty())
        {
            if (std::stol(faceData[1]) < 0)
                uvIdx = uvs.size() + 1 + std::stol(faceData[1]);
            else
                uvIdx = std::stoul(faceData[1]);
        }

        if (faceData.size() > 2 && !faceData[2].empty())
        {
            if (std::stol(faceData[2]) < 0)
                normalIdx = normals.size() + 1 + std::stol(faceData[2]);
            else
                normalIdx = std::stoul(faceData[2]);
        }

        return {
            posIdx > 0 ? positions[posIdx - 1] : Vector3(),
            normalIdx > 0 ? normals[normalIdx - 1] : Vector3(),
            uvIdx > 0 ? uvs[uvIdx - 1] : Vector2()
        };
    }

    uint32_t Mesh::addVertex(Vertex vertex)
    {
        uint32_t vertexIdx;

#if IGNORE_DUPLICATES
        vertexIdx = static_cast<uint32_t>(m_vertices.size());
        m_vertices.push_back(vertex);
#else
        bool isDuplicate = false;

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
#endif // IGNORE_DUPLICATES

        return vertexIdx;
    }

    void Mesh::parseFace(const std::string& line, const std::vector<Vector3>& positions, const std::vector<Vector3>& normals,
                          const std::vector<Vector2>& uvs)
    {
        const auto faceStrings = LibGL::Utility::splitString(line, " ", true);

        const std::vector<size_t> indices = getIndices(faceStrings.size());

        for (const size_t faceIdx : indices)
            m_indices.push_back(addVertex(parseVertex(faceStrings[faceIdx], positions, normals, uvs)));
    }
}

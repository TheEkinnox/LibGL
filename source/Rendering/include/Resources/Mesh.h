#pragma once
#include "Core/Vertex.h"
#include "Core/VertexAttributes.h"

#include "Resources/IResource.h"

#include "Vector/Vector2.h"
#include "Vector/Vector3.h"

namespace LibGL::Rendering::Resources
{
    class Mesh : public LibGL::Resources::IResource
    {
    public:
        using IResource::load;

        /**
         * \brief Loads the model from the given file
         * \param fileName The path of the model to load
         * \return True if the model was successfully loaded. False otherwise.
         */
        bool load(const char* fileName) override;

        /**
         * \brief Initializes the model
         * \return True if the model was successfully initialized. False otherwise.
         */
        bool init() override;

        /**
         * \brief Renders the model on screen
         */
        void draw() const;

    protected:
        std::vector<Vertex>   m_vertices;
        std::vector<uint32_t> m_indices;

        static LibMath::Vector3 parseVector3(const std::string& vec3Str);
        static LibMath::Vector2 parseVector2(const std::string& vec2Str);

        static std::vector<size_t> getIndices(size_t vertexCount);

        static Vertex parseVertex(const std::string&                   str,
                                  const std::vector<LibMath::Vector3>& positions,
                                  const std::vector<LibMath::Vector3>& normals,
                                  const std::vector<LibMath::Vector2>& uvs);

        uint32_t addVertex(Vertex vertex);

        void parseFace(const std::string&                   line,
                       const std::vector<LibMath::Vector3>& positions,
                       const std::vector<LibMath::Vector3>& normals,
                       const std::vector<LibMath::Vector2>& uvs);

    private:
        VertexAttributes m_vao;
    };
}

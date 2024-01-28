#include "LowRenderer/Model.h"

#include "LowRenderer/Camera.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

using namespace LibMath;
using namespace LibGL::Rendering::Resources;

namespace LibGL::Rendering
{
    Model::Model(Entity* parent, const Mesh& mesh, Material material)
        : Entity(parent, Transform()), m_mesh(&mesh), m_material(std::move(material))
    {
    }

    const Mesh* Model::getMesh() const
    {
        return m_mesh;
    }

    void Model::setMesh(const Mesh& mesh)
    {
        m_mesh = &mesh;
    }

    Material Model::getMaterial() const
    {
        return m_material;
    }

    Material& Model::getMaterial()
    {
        return m_material;
    }

    void Model::draw(const Matrix4x4& viewProjMat, Shader* shaderOverride) const
    {
        m_material.use();

        Shader&       shader   = shaderOverride ? *shaderOverride : m_material.getShader();
        const Matrix4 modelMat = getWorldMatrix();

        shader.setUniformMat4("u_mvp", viewProjMat * modelMat);
        shader.setUniformMat4("u_modelMat", modelMat);
        shader.setUniformMat4("u_normalMat", modelMat.inverse().transposed());

        m_mesh->draw();

        Shader::unbind();
        Texture::unbind();
    }
}

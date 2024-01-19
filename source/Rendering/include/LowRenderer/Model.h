#pragma once
#include <Entity.h>

#include "Core/IDrawable.h"

#include "Resources/Material.h"
#include "Resources/Mesh.h"

namespace LibGL::Rendering
{
    class Model : public Entity, public IDrawable
    {
    public:
        Model() = delete;
        explicit Model(Entity* parent, const Resources::Mesh& mesh, Resources::Material material);

        Model(const Model& other) = default;
        Model(Model&& other) noexcept = default;

        ~Model() override = default;

        Model& operator=(const Model& other) = default;
        Model& operator=(Model&& other) noexcept = default;

        /**
         * \brief Gets the model's mesh
         * \return The model's current mesh
         */
        const Resources::Mesh* getMesh() const;

        /**
         * \brief Sets the model's mesh
         * \param mesh The model's new mesh
         */
        void setMesh(const Resources::Mesh& mesh);

        /**
         * \brief Gets the model's material
         * \return A copy of the model's material
         */
        Resources::Material getMaterial() const;

        /**
         * \brief Gets the model's material
         * \return A reference to the model's material
         */
        Resources::Material& getMaterial();

        /**
         * \brief Draws the model using the given view-projection matrix
         * \param viewProjMat The target view-projection matrix
         * \param shaderOverride The (optional) shader override to use
         */
        void draw(const LibMath::Matrix4x4& viewProjMat, const Resources::Shader* shaderOverride) const override;

    private:
        const Resources::Mesh* m_mesh = nullptr;
        Resources::Material    m_material;
    };
}

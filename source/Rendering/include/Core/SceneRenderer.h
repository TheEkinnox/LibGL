#pragma once
#include "Scene.h"
#include "Core/Renderer.h"
#include "Resources/Shader.h"

namespace LibGL::Rendering
{
    class IDrawable;

    class SceneRenderer
    {
    public:
        SceneRenderer() = default;

        void init(const LibGL::Resources::Scene& scene);
        void render(const LibMath::Matrix4x4& viewProjMat, Resources::Shader* shaderOverride) const;

    private:
        std::vector<const IDrawable*> m_drawables;

        void init(const Entity& entity);
    };
}

#include "Core/SceneRenderer.h"

#include "Core/IDrawable.h"

using namespace LibMath;
using namespace LibGL::Resources;

namespace LibGL::Rendering
{
    void SceneRenderer::init(const Scene& scene)
    {
        m_drawables.clear();

        const auto entities = scene.getNodes();

        for (const auto& entity : entities)
            init(*entity);
    }

    void SceneRenderer::render(const Matrix4x4& viewProjMat, Resources::Shader* shaderOverride) const
    {
        if (m_drawables.empty())
            return;

        for (const IDrawable* drawable : m_drawables)
        {
            if (drawable)
                drawable->draw(viewProjMat, shaderOverride);
        }
    }

    void SceneRenderer::init(const Entity& entity)
    {
        if (!entity.isActive())
            return;

        if (const IDrawable* drawable = dynamic_cast<const IDrawable*>(&entity))
            m_drawables.push_back(drawable);

        const auto children = entity.getChildren();

        for (const auto& child : children)
            init(reinterpret_cast<const Entity&>(*child));
    }
}

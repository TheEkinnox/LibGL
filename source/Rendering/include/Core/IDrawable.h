#pragma once
#include "Matrix/Matrix4.h"
#include "Resources/Shader.h"

namespace LibGL::Rendering
{
    class IDrawable
    {
    public:
        virtual ~IDrawable() = default;

        /**
         * \brief Draws the object using the given view-projection matrix
         * \param viewProjMat The target view-projection matrix
         * \param shaderOverride The (optional) shader override to use
         */
        virtual void draw(const LibMath::Matrix4x4& viewProjMat, Resources::Shader* shaderOverride) const = 0;
    };
}

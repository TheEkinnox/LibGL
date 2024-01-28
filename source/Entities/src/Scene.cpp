#include "Scene.h"

namespace LibGL::Resources
{
    void Scene::update()
    {
        for (const auto& node : getNodes())
            node->update();
    }
}

#include "DemoContext.h"

#include "Utility/ServiceLocator.h"
#include <Angle.h>

using namespace LibGL::Application;
using namespace LibGL::Rendering;
using namespace LibGL::Resources;
using namespace LibGL::Utility;

using namespace LibMath;
using namespace Literal;

#define CAM_NEAR .1f
#define CAM_FAR 14.f

namespace LibGL::Demo
{
    DemoContext::DemoContext(const int windowWidth, const int windowHeight, const char* title)
        : IContext(windowWidth, windowHeight, title),
          m_threadPool(std::make_unique<ThreadPool>()),
          m_inputManager(std::make_unique<InputManager>(*m_window)),
          m_resourceManager(std::make_unique<ResourceManager>()),
          m_renderer(std::make_unique<Renderer>()),
          m_sceneRenderer(std::make_unique<SceneRenderer>()),
          m_camera(std::make_unique<Camera>(nullptr, Transform(),
              perspectiveProjection(90_deg, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), CAM_NEAR,
                  CAM_FAR)))
    {
        const auto resizeFunc = [&](const Window::dimensions_t size)
        {
            m_renderer->setViewPort(0, 0, size.m_x, size.m_y);
            m_camera->setProjectionMatrix(perspectiveProjection(90_deg,
                static_cast<float>(size.m_x) / static_cast<float>(size.m_y),
                CAM_NEAR, CAM_FAR));
        };

        m_window->m_resizeEvent.subscribe(resizeFunc);
        m_camera->setClearColor(Color::skyBlue).setClearColorBuffer(true).setClearDepthBuffer(true);
        Camera::setCurrent(*m_camera);

        ServiceLocator::provide<ThreadPool>(*m_threadPool);
        ServiceLocator::provide<InputManager>(*m_inputManager);
        ServiceLocator::provide<ResourceManager>(*m_resourceManager);
        ServiceLocator::provide<Renderer>(*m_renderer);
        ServiceLocator::provide<SceneRenderer>(*m_sceneRenderer);

        // Enable back-face culling
        m_renderer->setCapability(ERenderingCapability::CULL_FACE, true);
        m_renderer->setCullFace(ECullFace::BACK);

        // Enable depth-test
        m_renderer->setCapability(ERenderingCapability::DEPTH_TEST, true);

        // Enable alpha-blending
        m_renderer->setCapability(ERenderingCapability::BLEND, true);
    }

    void DemoContext::update()
    {
        m_inputManager->update();
        IContext::update();
    }
}

#include "DemoContext.h"

#include "Utility/ServiceLocator.h"

using namespace LibGL::Application;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;

namespace LibGL::Demo
{
	DemoContext::DemoContext(const int windowWidth, const int windowHeight,
		const char* title) : IContext(windowWidth, windowHeight, title),
		m_inputManager(std::make_unique<InputManager>(*m_window)),
		m_resourceManager(std::make_unique<ResourceManager>()),
		m_renderer(std::make_unique<Renderer>())
	{
		ServiceLocator::provide<InputManager>(*m_inputManager);
		ServiceLocator::provide<ResourceManager>(*m_resourceManager);
		ServiceLocator::provide<Renderer>(*m_renderer);

		m_renderer->setClearColor(0.f, 0.f, 1.f);

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
		IContext::update();
		m_inputManager->updateMouse();
	}
}

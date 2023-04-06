#include "DemoContext.h"

#include "Utility/ServiceLocator.h"

using namespace LibGL::Application;
using namespace LibGL::Resources;

namespace LibGL::Demo
{
	DemoContext::DemoContext(const int windowWidth, const int windowHeight,
		const char* title) : IContext(windowWidth, windowHeight, title),
		m_inputManager(std::make_unique<InputManager>(*m_window)),
		m_resourceManager(std::make_unique<ResourceManager>())
	{
		ServiceLocator::provide<InputManager>(*m_inputManager);
		ServiceLocator::provide<ResourceManager>(*m_resourceManager);
	}

	void DemoContext::update()
	{
		IContext::update();
		m_inputManager->updateMouse();
	}
}

#pragma once
#include "IContext.h"
#include "InputManager.h"
#include "Resources/ResourcesManager.h"

namespace LibGL::Demo
{
	class DemoContext final : public Application::IContext
	{
	public:
		DemoContext(int windowWidth, int windowHeight, const char* title);

		void update() override;

		std::unique_ptr<Application::InputManager>	m_inputManager;
		std::unique_ptr<Resources::ResourceManager>	m_resourceManager;
	};
}

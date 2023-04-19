#pragma once
#include "LowRenderer/Camera.h"
#include "LowRenderer/Light.h"
#include "IApplication.h"

namespace LibGL::Rendering
{
	class Mesh;
}

namespace LibGL::Demo
{
	class DemoApp final : public Application::IApplication
	{
	public:
		/**
		 * \brief Creates a GLFW window and initializes OpenGL with GLAD
		 * \param windowWidth The created window's width
		 * \param windowHeight The created window's height
		 * \param title The created window's title
		 */
		DemoApp(int windowWidth, int windowHeight, const char* title);

		/**
		 * \brief Disable the Application's copy constructor
		 */
		DemoApp(const DemoApp&) = delete;

		/**
		 * \brief Disable the Application's move constructor
		 */
		DemoApp(DemoApp&&) = delete;

		/**
		 * \brief Clears the application's allocated data
		 */
		~DemoApp() override = default;

		/**
		 * \brief Disable the Application's copy assignment operator
		 */
		DemoApp& operator=(const DemoApp&) = delete;

		/**
		 * \brief Disable the Application's move assignment operator
		 */
		DemoApp& operator=(DemoApp&&) = delete;

	private:
		static constexpr int NB_POINT_LIGHTS = 4;

		Resources::Scene			m_scene;
		Rendering::Light			m_ambientLight;
		Rendering::PointLight		m_pointLights[NB_POINT_LIGHTS];
		Rendering::DirectionalLight	m_dirLight;
		Rendering::SpotLight		m_spotLight;
		Rendering::Mesh*			m_controllableMesh;

		/**
		 * \brief Function to call on the application start
		 */
		void onStart() override;

		/**
		 * \brief Function to call on every main loop cycle
		 */
		void onUpdate() override;

		/**
		 * \brief Function to call on the application stop
		 */
		void onStop() override {}

		/**
		 * \brief Creates the 3d environment
		 */
		void createScene();

		/**
		 * \brief Loads or creates a new lit shader from the given file
		 */
		static Resources::Shader* setupLitShader(const std::string& fileName);

		/**
		 * \brief Loads or creates a new unlit shader from the given file
		 */
		static Resources::Shader* setupUnlitShader(const std::string& fileName);

		/**
		 * \brief Processes user's keyboard input
		 */
		void handleKeyboard();

		/**
		 * \brief Processes user's mouse input
		 */
		void handleMouse() const;

		/**
		 * \brief Handles the application's rendering
		 */
		void render();

		/**
		 * \brief Update the lighting data for the shader with the given file name
		 */
		void updateLitShader(const std::string& fileName) const;
	};
}

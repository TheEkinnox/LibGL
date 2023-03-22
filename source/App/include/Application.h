#pragma once
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Mesh.h"
#include "ResourcesManager.h"
#include "Utility/Timer.h"

namespace My
{
	using namespace Utility;
	using namespace Resources;
	using namespace Rendering;

	namespace Exceptions
	{
		class WindowCreationFailed : public std::runtime_error
		{
		public:
			WindowCreationFailed()
				: runtime_error("Window creation failed") {}

			explicit WindowCreationFailed(const std::string& message)
				: runtime_error(message.c_str()) {}

			explicit WindowCreationFailed(const char* message)
				: runtime_error(message) {}
		};

		class GLADInitFailed : public std::runtime_error
		{
		public:
			GLADInitFailed()
				: runtime_error("Glad initialization failed") {}

			explicit GLADInitFailed(const std::string& message)
				: runtime_error(message.c_str()) {}

			explicit GLADInitFailed(const char* message)
				: runtime_error(message) {}
		};
	}

	class Application
	{
	public:
		/**
		 * \brief Creates a GLFW window and initializes OpenGL with GLAD
		 * \param windowWidth The created window's width
		 * \param windowHeight The created window's height
		 * \param title The created window's title
		 */
		Application(int windowWidth, int windowHeight, const char* title);

		/**
		 * \brief Clears the application's previously allocated resources
		 */
		~Application();

		/**
		 * \brief Starts the application's main loop
		 */
		void run();

	private:
		static ResourceManager	m_resourceManager;
		GLFWwindow*				m_window;
		Camera					m_camera;
		Timer					m_timer;
		std::vector<Mesh>		m_meshes;

		/**
		 * \brief Creates the 3d environment
		 */
		void createScene();

		/**
		 * \brief Processes user input
		 */
		void processInput();

		/**
		 * \brief Handles the application's rendering
		 */
		void render() const;

		/**
		 * \brief OpenGL frame buffer size change callback
		 * \param window The window for which the frame buffer size has hanged
		 * \param width The window's frame buffer's new width
		 * \param height The window's frame buffer's new height
		 */
		static void onFrameBufferResize(GLFWwindow* window, int width, int height);

		/**
		 * \brief OpenGL debug output callback
		 * \param source The debug message's source
		 * \param type The debug message's type
		 * \param id The debug message's id (error/warning code, etc...)
		 * \param severity The debug message's severity
		 * \param length The debug message's length
		 * \param message The debug message
		 * \param userParam Addition user parameters
		 */
		static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
		                                   GLenum severity, GLsizei length, const GLchar* message,
		                                   const void* userParam);
	};
}

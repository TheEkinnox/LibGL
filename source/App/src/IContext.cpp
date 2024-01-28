#include "IContext.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Debug/Log.h"
#include "Utility/ServiceLocator.h"

using namespace LibGL::Utility;
using namespace LibGL::Application::Exceptions;

namespace LibGL::Application
{
	/**
	 * \brief OpenGL debug output callback
	 * (in the cpp to avoid an unnecessary include of glad.h in the header)
	 * \param source The debug message's source
	 * \param type The debug message's type
	 * \param id The debug message's id (error/warning code, etc...)
	 * \param severity The debug message's severity
	 * \param length The debug message's length
	 * \param message The debug message
	 * \param userParam Addition user parameters
	 */
	void APIENTRY glDebugOutput(uint32_t source, uint32_t type, uint32_t id,
		uint32_t severity, int length, const char* message,
		const void* userParam);

	IContext::IContext(const int windowWidth, const int windowHeight, const char* title) :
		m_window(std::make_unique<Window>(Window::dimensions_t(windowWidth, windowHeight), title)),
		m_timer(std::make_unique<Timer>())
	{
		m_window->makeCurrentContext();

		// Load GLAD's OpenGL function pointers
		if (!gladLoadGL(glfwGetProcAddress))
		{
			DEBUG_LOG("Failed to initialize GLAD\n");
			throw GLADInitFailed("Failed to initialize GLAD");
		}

		// Setup OpenGL debugging
		bindDebugCallback();

		ServiceLocator::provide<Timer>(*m_timer);
		ServiceLocator::provide<Window>(*m_window);
	}

	IContext::~IContext()
	{
		// Clear all previously allocated GLFW resources
		glfwTerminate();
	}

	void IContext::update()
	{
		m_timer->update();
		glfwPollEvents();
	}

	void IContext::bindDebugCallback()
	{
		GLint flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}

	void APIENTRY glDebugOutput(const uint32_t source, const uint32_t type,
	                                      const uint32_t id, const uint32_t severity, const int,
	                                      const char* message, const void*)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		Debug::Log::print("---------------\n");
		Debug::Log::print("Debug message (%u): %s\n", id, message);

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			Debug::Log::print("Source: API");
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			Debug::Log::print("Source: Window System"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			Debug::Log::print("Source: Shader Compiler");
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			Debug::Log::print("Source: Third Party");
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			Debug::Log::print("Source: Application");
			break;
		case GL_DEBUG_SOURCE_OTHER:
		default:
			Debug::Log::print("Source: Other");
			break;
		}

		Debug::Log::print("\n");

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			Debug::Log::print("Type: Error");
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			Debug::Log::print("Type: Deprecated Behaviour");
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			Debug::Log::print("Type: Undefined Behaviour");
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			Debug::Log::print("Type: Portability");
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			Debug::Log::print("Type: Performance");
			break;
		case GL_DEBUG_TYPE_MARKER:
			Debug::Log::print("Type: Marker");
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			Debug::Log::print("Type: Push Group");
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			Debug::Log::print("Type: Pop Group");
			break;
		case GL_DEBUG_TYPE_OTHER:
		default:
			Debug::Log::print("Type: Other");
			break;
		}

		Debug::Log::print("\n");

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			Debug::Log::print("Severity: high");
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			Debug::Log::print("Severity: medium");
			break;
		case GL_DEBUG_SEVERITY_LOW:
			Debug::Log::print("Severity: low");
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			Debug::Log::print("Severity: notification");
			break;
		default:
			Debug::Log::print("Severity: unknown");
			break;
		}

		Debug::Log::print("\n\n");
	}
}

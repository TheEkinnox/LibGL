#include "Debug/Log.h"
#include "Application.h"

#include "Shader.h"
#include "Debug/Assertion.h"

#define DEFAULT_SHADER "shaders/Basic.glsl"

namespace My
{
	using namespace Exceptions;

	ResourceManager Application::m_resourceManager = ResourceManager();

	Application::Application(const int windowWidth, const int windowHeight, const char* title)
	{
		// Initialize and configure glfw
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
		// required to compile on OS X
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		// Initialize debugger
		Debug::Log::openFile("debug.log");

		// Create glfw window
		m_window = glfwCreateWindow(windowWidth, windowHeight, title, nullptr, nullptr);

		if (m_window == nullptr)
		{
			DEBUG_LOG("Failed to create GLFW window\n");
			glfwTerminate();
			throw WindowCreationFailed("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, onFrameBufferResize);

		// Load GLAD's OpenGL function pointers
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			DEBUG_LOG("Failed to initialize GLAD\n");
			glfwTerminate();
			throw GLADInitFailed("Failed to initialize GLAD");
		}

		// Setup OpenGL debugging
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

	Application::~Application()
	{
		// Clear all previously allocated GLFW resources
		glfwTerminate();
	}

	void Application::run() const
	{
		Shader* shader = m_resourceManager.create<Shader>(DEFAULT_SHADER);
		ASSERT(shader->setVertexShader());
		ASSERT(shader->link());

		// Run main loop
		while (!glfwWindowShouldClose(m_window))
		{
			// Handle inputs
			processInput();

			// Handle rendering
			render();

			// Swap glfw buffers and poll IO events (keys, mouse etc...)
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
	}

	void Application::processInput() const
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);
	}

	void Application::render()
	{
		glClearColor(0.f, 0.f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		const Shader* shader = m_resourceManager.get<Shader>(DEFAULT_SHADER);
		shader->use();
	}

	void Application::onFrameBufferResize(GLFWwindow*,
		const int width, const int height)
	{
		// make sure the viewport matches the new window dimensions
		glViewport(0, 0, width, height);
	}

	void APIENTRY Application::glDebugOutput(const GLenum source, const GLenum type,
		const GLuint id, const GLenum severity, const GLsizei,
		const GLchar* message, const void*)
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

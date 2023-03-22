#include "Debug/Log.h"
#include "Application.h"

#include "Model.h"
#include "Shader.h"
#include "Debug/Assertion.h"
#include "Maths/Angle.h"

#define DEFAULT_SHADER "shaders/Default.glsl"
#define MOVE_SPEED .5f
#define ROTATION_SPEED 30

namespace My
{
	using namespace Exceptions;
	using namespace Literal;

	ResourceManager Application::m_resourceManager = ResourceManager();

	Application::Application(const int windowWidth, const int windowHeight, const char* title)
		: m_camera(Transform(),
			Matrix4::perspectiveProjection(90_deg,
				static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
				0.1f, 8.f))
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

	void Application::run()
	{
		createScene();

		Shader* shader = m_resourceManager.create<Shader>(DEFAULT_SHADER);
		ASSERT(shader != nullptr);
		ASSERT(shader->setVertexShader());
		ASSERT(shader->setFragmentShader());
		ASSERT(shader->link());

		// Run main loop
		while (!glfwWindowShouldClose(m_window))
		{
			// Update the timer
			m_timer.update();

			// Handle inputs
			processInput();

			// Handle rendering
			render();

			// Swap glfw buffers and poll IO events (keys, mouse etc...)
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
	}

	void Application::createScene()
	{
		// Load the models
		const Model* floorModel = m_resourceManager.getOrCreate<Model>("meshes/floor.obj");
		ASSERT(floorModel != nullptr);

		const Model* headModel = m_resourceManager.getOrCreate<Model>("meshes/boggie/head.obj");
		ASSERT(headModel != nullptr);

		const Model* diabloModel = m_resourceManager.getOrCreate<Model>("meshes/diablo3_pose/diablo3_pose.obj");
		ASSERT(diabloModel != nullptr);

		// Place the meshes
		Mesh mesh(*floorModel);
		m_meshes.push_back(mesh);

		mesh = Mesh(*headModel);
		mesh.m_transform.setPosition(Vector3(- 1, 1, -1));
		m_meshes.push_back(mesh);

		mesh = Mesh(*diabloModel);
		mesh.m_transform.setPosition(Vector3(1, 0, -1));
		mesh.m_transform.setScale(Vector3(.5f));
		m_meshes.push_back(mesh);
	}

	void Application::processInput()
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);

		if (glfwGetKey(m_window, GLFW_KEY_R))
			m_camera.setPosition(Vector3::zero());

		// Movement
		float moveSpeed = MOVE_SPEED * m_timer.getDeltaTime();

		if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT))
			moveSpeed *= 1.5f;

		if (glfwGetKey(m_window, GLFW_KEY_W))
			m_camera.translate(m_camera.forward() * moveSpeed);

		if (glfwGetKey(m_window, GLFW_KEY_S))
			m_camera.translate(-m_camera.forward() * moveSpeed);

		if (glfwGetKey(m_window, GLFW_KEY_Q))
			m_camera.translate(-m_camera.right() * moveSpeed);

		if (glfwGetKey(m_window, GLFW_KEY_E))
			m_camera.translate(m_camera.right() * moveSpeed);

		// Rotation
		if (glfwGetKey(m_window, GLFW_KEY_UP))
			m_camera.rotate(ROTATION_SPEED * Vector3::right() * m_timer.getDeltaTime());

		if (glfwGetKey(m_window, GLFW_KEY_DOWN))
			m_camera.rotate(-ROTATION_SPEED * Vector3::right() * m_timer.getDeltaTime());

		if (glfwGetKey(m_window, GLFW_KEY_A) ||
			glfwGetKey(m_window, GLFW_KEY_LEFT))
			m_camera.rotate(-ROTATION_SPEED * Vector3::up() * m_timer.getDeltaTime());

		if (glfwGetKey(m_window, GLFW_KEY_D) ||
			glfwGetKey(m_window, GLFW_KEY_RIGHT))
			m_camera.rotate(ROTATION_SPEED * Vector3::up() * m_timer.getDeltaTime());
	}

	void Application::render() const
	{
		glClearColor(0.f, 0.f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		const Shader* shader = m_resourceManager.get<Shader>(DEFAULT_SHADER);
		shader->use();

		const GLint mvpMatUniformLoc = shader->getUniformLocation("mvp");
		const Matrix4 viewProjMat = m_camera.getViewProjectionMatrix();

		for (const auto& mesh : m_meshes)
		{
			const Matrix4 mvpMat = viewProjMat * mesh.m_transform.getMatrix();
			
			glUniformMatrix4fv(mvpMatUniformLoc, 1, GL_TRUE, mvpMat.getArray());

			mesh.draw();
		}
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

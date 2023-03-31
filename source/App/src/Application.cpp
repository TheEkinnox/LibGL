// Log.h must be included before glad - minwindef causes conflicts with APIENTRY
#include "Debug/Log.h"
#include "Application.h"

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Debug/Assertion.h"
#include "Angle.h"
#include "Arithmetic.h"
#include "Trigonometry.h"
#include "Utility/utility.h"

#define CAM_NEAR .1f
#define CAM_FAR 14.f

#define MOVE_SPEED .5f
#define ROTATION_SPEED 30
#define MOUSE_SENSITIVITY .8f

namespace My
{
	using namespace Exceptions;
	using namespace Literal;

	ResourceManager Application::m_resourceManager = ResourceManager();

	Application::Application(const int windowWidth, const int windowHeight, const char* title)
		: m_camera(nullptr, Transform(),
			Matrix4::perspectiveProjection(90_deg,
				static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
				CAM_NEAR, CAM_FAR)), m_isFirstMouse(true)
	{
		Camera::setCurrent(m_camera);

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

		// Enable back-face culling
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Enable depth-test
		glEnable(GL_DEPTH_TEST);

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

		// Run main loop
		while (!glfwWindowShouldClose(m_window))
		{
			// Update the timer
			m_timer.update();

			// Handle keyboard and mouse inputs
			handleKeyboard();
			handleMouse();

			// Handle rendering
			render();

			// Swap glfw buffers and poll IO events (keys, mouse etc...)
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
	}

	void Application::createScene()
	{
		//const Shader* shader = setupUnlitShader("shaders/Default.glsl");
		const Shader* shader = setupLitShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Load the models
		const Model* floorModel = m_resourceManager.getOrCreate<Model>("meshes/floor.obj");
		ASSERT(floorModel != nullptr);

		const Texture* floorDiffuse = m_resourceManager.getOrCreate<Texture>("textures/container2.png");
		ASSERT(floorDiffuse != nullptr);

		const Texture* floorSpecular = m_resourceManager.getOrCreate<Texture>("textures/container2_specular.png");
		ASSERT(floorSpecular != nullptr);

		const Model* headModel = m_resourceManager.getOrCreate<Model>("meshes/boggie/head.obj");
		ASSERT(headModel != nullptr);

		const Texture* headDiffuse = m_resourceManager.getOrCreate<Texture>("meshes/boggie/head_diffuse.tga");
		ASSERT(headDiffuse != nullptr);

		const Texture* headSpecular = m_resourceManager.getOrCreate<Texture>("meshes/boggie/head_spec.tga");
		ASSERT(headSpecular != nullptr);

		const Model* diabloModel = m_resourceManager.getOrCreate<Model>("meshes/diablo3_pose/diablo3_pose.obj");
		ASSERT(diabloModel != nullptr);

		const Texture* diabloDiffuse = m_resourceManager.getOrCreate<Texture>("meshes/diablo3_pose/diablo3_pose_diffuse.tga");
		ASSERT(diabloDiffuse != nullptr);

		const Texture* diabloSpecular = m_resourceManager.getOrCreate<Texture>("meshes/diablo3_pose/diablo3_pose_spec.tga");
		ASSERT(diabloSpecular != nullptr);

		const Material floorMat(*shader, floorDiffuse, floorSpecular, 32.f);
		const Material headMat(*shader, headDiffuse, headSpecular, 8.f);
		const Material diabloMat(*shader, diabloDiffuse, diabloSpecular, 16.f);

		// Place the meshes
		Mesh mesh(nullptr, *floorModel, floorMat);
		mesh.setScale(Vector3( 7, 1, 7 ));
		m_scene.addNode(mesh);

		mesh = Mesh(nullptr, *headModel, headMat);
		mesh.setPosition(Vector3(-1, -1, -1.f));
		m_scene.addNode(mesh);

		mesh = Mesh(nullptr, *diabloModel, diabloMat);
		mesh.setPosition(Vector3(.5f, -.5f, -1.f));
		mesh.setScale(Vector3(.5f));
		m_scene.addNode(mesh);

		// Setup the lights
		m_dirLight =
		{
			{
				Vector4(.05f, .05f, .05f, 1),
				Vector4(.1f, .1f, .1f, 1),
				Vector4(.1f, .1f, .1f, 1),
			},
			Vector3(0, -1, 0)
		};

		const Light pointLightBase
		{
				Vector4(.1f, 0.f, .1f, 1),
				Vector4(0.4f, 0.f, .4f, 1),
				Vector4(0.5f, 0.f, .5f, 1),
		};

		m_pointLights[0] =
		{
			pointLightBase,
			{ -1, 0, 1 },
			{ 1.f, .7f, 1.8f }
		};

		m_pointLights[1] =
		{
			pointLightBase,
			{ 1, 0, 1 },
			{ 1.f, .7f, 1.8f }
		};

		m_pointLights[2] =
		{
			pointLightBase,
			{ 0, 0, -1 },
			{ 1.f, .7f, 1.8f }
		};

		m_spotLight =
		{
			{
				Vector4(.15f, .15f, .15f, 1),
				Vector4(.6f, .6f, .6f, 1),
				Vector4(.75f, .75f, .75f, 1),
			},
			m_camera.getPosition(),
			m_camera.forward(),
			{ 1.f, .35f, .44f },
			cos(25_deg),
			cos(30_deg),
		};
	}

	Shader* Application::setupLitShader(const std::string& fileName)
	{
		Shader* shader = m_resourceManager.get<Shader>(fileName);

		if (shader == nullptr)
		{
			shader = m_resourceManager.create<Shader>(fileName);

			ASSERT(shader != nullptr);
			ASSERT(shader->setVertexShader());
			ASSERT(shader->setFragmentShader());
			ASSERT(shader->link());
		}

		return shader;
	}

	Shader* Application::setupUnlitShader(const std::string& fileName)
	{
		Shader* shader = m_resourceManager.get<Shader>(fileName);

		if (shader == nullptr)
		{
			shader = m_resourceManager.create<Shader>(fileName);

			ASSERT(shader != nullptr);
			ASSERT(shader->setVertexShader());
			ASSERT(shader->setFragmentShader());
			ASSERT(shader->link());
		}

		return shader;
	}

	void Application::handleKeyboard()
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);

		if (glfwGetKey(m_window, GLFW_KEY_R))
		{
			m_camera.setPosition(Vector3::zero());
			m_camera.setRotation(Vector3::zero());
		}

		// Dir light rotation
		if (glfwGetKey(m_window, GLFW_KEY_I))
			m_dirLight.m_direction.rotate(Degree(ROTATION_SPEED * m_timer.getDeltaTime()), Vector3::right());

		if (glfwGetKey(m_window, GLFW_KEY_J))
			m_dirLight.m_direction.rotate(Degree(-ROTATION_SPEED * m_timer.getDeltaTime()), Vector3::up());

		if (glfwGetKey(m_window, GLFW_KEY_K))
			m_dirLight.m_direction.rotate(Degree(-ROTATION_SPEED * m_timer.getDeltaTime()), Vector3::right());

		if (glfwGetKey(m_window, GLFW_KEY_L))
			m_dirLight.m_direction.rotate(Degree(ROTATION_SPEED * m_timer.getDeltaTime()), Vector3::up());

		// Movement
		float moveSpeed = MOVE_SPEED * m_timer.getDeltaTime();

		if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL)
			|| glfwGetKey(m_window, GLFW_KEY_RIGHT_CONTROL))
			moveSpeed *= 1.5f;

		if (glfwGetKey(m_window, GLFW_KEY_W))
			m_camera.translate(m_camera.forward() * moveSpeed);

		if (glfwGetKey(m_window, GLFW_KEY_S))
			m_camera.translate(m_camera.back() * moveSpeed);

		if (glfwGetKey(m_window, GLFW_KEY_A))
			m_camera.translate(m_camera.left() * moveSpeed);

		if (glfwGetKey(m_window, GLFW_KEY_D))
			m_camera.translate(m_camera.right() * moveSpeed);

		if (glfwGetKey(m_window, GLFW_KEY_SPACE))
			m_camera.translate(Vector3::up() * moveSpeed);

		if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT)
			|| glfwGetKey(m_window, GLFW_KEY_RIGHT_SHIFT))
			m_camera.translate(Vector3::down() * moveSpeed);

		// Rotation
		if (glfwGetKey(m_window, GLFW_KEY_UP))
			m_camera.rotate(ROTATION_SPEED * Vector3::right() * m_timer.getDeltaTime());

		if (glfwGetKey(m_window, GLFW_KEY_DOWN))
			m_camera.rotate(-ROTATION_SPEED * Vector3::right() * m_timer.getDeltaTime());

		if (glfwGetKey(m_window, GLFW_KEY_Q) ||
			glfwGetKey(m_window, GLFW_KEY_LEFT))
			m_camera.rotate(-ROTATION_SPEED * Vector3::up() * m_timer.getDeltaTime());

		if (glfwGetKey(m_window, GLFW_KEY_E) ||
			glfwGetKey(m_window, GLFW_KEY_RIGHT))
			m_camera.rotate(ROTATION_SPEED * Vector3::up() * m_timer.getDeltaTime());

		m_spotLight.m_position = m_camera.getPosition();
		m_spotLight.m_direction = m_camera.forward();
	}

	void Application::handleMouse()
	{
		if (!glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) &&
			!glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_isFirstMouse = true;
			return;
		}

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		double mouseX, mouseY;
		glfwGetCursorPos(m_window, &mouseX, &mouseY);

		m_mousePos.m_x = static_cast<float>(mouseX);
		m_mousePos.m_y = static_cast<float>(mouseY);

		if (m_isFirstMouse)
		{
			m_lastMousePos = m_mousePos;
			m_isFirstMouse = false;
		}

		m_mouseDelta = m_mousePos - m_lastMousePos;
		m_lastMousePos = m_mousePos;

		//update camera rotation
		const float rotationSpeed = ROTATION_SPEED * MOUSE_SENSITIVITY
			* m_timer.getDeltaTime();

		Vector3 camRotation = m_camera.getRotation();
		camRotation.m_x += -m_mouseDelta.m_y * rotationSpeed;
		camRotation.m_y += m_mouseDelta.m_x * rotationSpeed;

		camRotation.m_x = clamp(camRotation.m_x, -80.f, 80.f);
		camRotation.m_y = wrap(camRotation.m_y, 0.f, 360.f);

		m_camera.setRotation(camRotation);
	}

	void Application::render()
	{
		glClearColor(0.f, 0.f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updateLitShader("shaders/Lit.glsl");

		m_scene.update();
	}

	void Application::updateLitShader(const std::string& fileName) const
	{
		const Shader* shader = setupLitShader(fileName);

		if (shader == nullptr)
			return;

		shader->use();

		// Setup camera
		const GLint uniformLoc = shader->getUniformLocation("viewPos");
		glUniform3fv(uniformLoc, 1, m_camera.getGlobalTransform().getPosition().getArray());

		// Setup directional light
		m_dirLight.setupUniform("dirLight", *shader);

		// Setup point lights
		for (size_t i = 0; i < NB_POINT_LIGHTS; i++)
		{
			const std::string prefix = formatString("pointLights[%i]", i);
			m_pointLights[i].setupUniform(prefix, *shader);
		}

		// Setup spot light
		m_spotLight.setupUniform("spotLight", *shader);

		Shader::unbind();
	}

	void Application::onFrameBufferResize(GLFWwindow*,
	                                      const int width, const int height)
	{
		// make sure the viewport matches the new window dimensions
		glViewport(0, 0, width, height);

		// Update the camera's projection matrix to use the new aspect ratio
		const float aspect = static_cast<float>(width) / static_cast<float>(height);
		const Matrix4 projMat = Matrix4::perspectiveProjection(90_deg, aspect,
			CAM_NEAR, CAM_FAR);

		Camera::getCurrent().setProjectionMatrix(projMat);
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

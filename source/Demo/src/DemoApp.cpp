#include "DemoApp.h"

#include "Resources/Model.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "LowRenderer/Mesh.h"
#include "Debug/Log.h"
#include "Debug/Assertion.h"
#include "Angle.h"
#include "Arithmetic.h"
#include "DemoContext.h"
#include "InputManager.h"
#include "Transform.h"
#include "Trigonometry.h"
#include "Utility/ServiceLocator.h"
#include "Utility/utility.h"

#define MOVE_SPEED .5f
#define ROTATION_SPEED 30.f
#define MOUSE_SENSITIVITY .8f

using namespace LibMath;
using namespace LibMath::Exceptions;
using namespace LibMath::Literal;
using namespace LibGL::Utility;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;
using namespace LibGL::Application;
using namespace LibGL::Application::Exceptions;

namespace LibGL::Demo
{

	DemoApp::DemoApp(const int windowWidth, const int windowHeight, const char* title)
		: IApplication(std::make_unique<DemoContext>(windowWidth, windowHeight, title))
	{
		// Initialize debugger
		Debug::Log::openFile("debug.log");
	}

	void DemoApp::onStart()
	{
		createScene();
	}

	void DemoApp::onUpdate()
	{
		// Handle keyboard and mouse inputs
		handleKeyboard();
		handleMouse();

		// Handle rendering
		render();

		// Swap render buffers
		LGL_SERVICE(Window).swapBuffers();
	}

	void DemoApp::createScene()
	{
		auto& resourceManager = LGL_SERVICE(ResourceManager);
		const auto& camera = Camera::getCurrent();

		//const Shader* shader = setupUnlitShader("shaders/Unlit.glsl");
		//const Shader* shader = setupUnlitShader("shaders/Normal.glsl");
		//const Shader* shader = setupUnlitShader("shaders/Basic.glsl");
		const Shader* shader = setupLitShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Load the models
		const Model* floorModel = resourceManager.getOrCreate<Model>("meshes/floor.obj");
		ASSERT(floorModel != nullptr);

		const Texture* floorDiffuse = resourceManager.getOrCreate<Texture>("textures/container2.png");
		ASSERT(floorDiffuse != nullptr);

		const Texture* floorSpecular = resourceManager.getOrCreate<Texture>("textures/container2_specular.png");
		ASSERT(floorSpecular != nullptr);

		const Model* headModel = resourceManager.getOrCreate<Model>("meshes/boggie/head.obj");
		ASSERT(headModel != nullptr);

		const Texture* headDiffuse = resourceManager.getOrCreate<Texture>("meshes/boggie/head_diffuse.tga");
		ASSERT(headDiffuse != nullptr);

		const Texture* headSpecular = resourceManager.getOrCreate<Texture>("meshes/boggie/head_spec.tga");
		ASSERT(headSpecular != nullptr);

		const Model* diabloModel = resourceManager.getOrCreate<Model>("meshes/diablo3_pose/diablo3_pose.obj");
		ASSERT(diabloModel != nullptr);

		const Texture* diabloDiffuse = resourceManager.getOrCreate<Texture>("meshes/diablo3_pose/diablo3_pose_diffuse.tga");
		ASSERT(diabloDiffuse != nullptr);

		const Texture* diabloSpecular = resourceManager.getOrCreate<Texture>("meshes/diablo3_pose/diablo3_pose_spec.tga");
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
			camera.getPosition(),
			camera.forward(),
			{ 1.f, .35f, .44f },
			cos(25_deg),
			cos(30_deg),
		};
	}

	Shader* DemoApp::setupLitShader(const std::string& fileName)
	{
		auto& resourceManager = LGL_SERVICE(ResourceManager);
		Shader* shader = resourceManager.get<Shader>(fileName);

		if (shader == nullptr)
		{
			shader = resourceManager.create<Shader>(fileName);

			ASSERT(shader != nullptr);
			ASSERT(shader->setVertexShader());
			ASSERT(shader->setFragmentShader());
			ASSERT(shader->link());
		}

		return shader;
	}

	Shader* DemoApp::setupUnlitShader(const std::string& fileName)
	{
		auto& resourceManager = LGL_SERVICE(ResourceManager);
		Shader* shader = resourceManager.get<Shader>(fileName);

		if (shader == nullptr)
		{
			shader = resourceManager.create<Shader>(fileName);

			ASSERT(shader != nullptr);
			ASSERT(shader->setVertexShader());
			ASSERT(shader->setFragmentShader());
			ASSERT(shader->link());
		}

		return shader;
	}

	void DemoApp::handleKeyboard()
	{
		const auto& inputManager = LGL_SERVICE(InputManager);
		auto& camera = Camera::getCurrent();

		if (inputManager.isKeyDown(EKey::KEY_ESCAPE))
			LGL_SERVICE(Window).setShouldClose(true);

		if (inputManager.isKeyDown(EKey::KEY_R))
		{
			camera.setPosition(Vector3::zero());
			camera.setRotation(Vector3::zero());
		}

		const float deltaTime = LGL_SERVICE(Timer).getDeltaTime();

		// Dir light rotation
		if (inputManager.isKeyDown(EKey::KEY_I))
			m_dirLight.m_direction.rotate(Degree(ROTATION_SPEED * deltaTime), Vector3::right());

		if (inputManager.isKeyDown(EKey::KEY_J))
			m_dirLight.m_direction.rotate(Degree(-ROTATION_SPEED * deltaTime), Vector3::up());

		if (inputManager.isKeyDown(EKey::KEY_K))
			m_dirLight.m_direction.rotate(Degree(-ROTATION_SPEED * deltaTime), Vector3::right());

		if (inputManager.isKeyDown(EKey::KEY_L))
			m_dirLight.m_direction.rotate(Degree(ROTATION_SPEED * deltaTime), Vector3::up());

		// Movement
		float moveSpeed = MOVE_SPEED * deltaTime;

		if (inputManager.isKeyDown(EKey::KEY_LEFT_CONTROL)
			|| inputManager.isKeyDown(EKey::KEY_RIGHT_CONTROL))
			moveSpeed *= 1.5f;

		if (inputManager.isKeyDown(EKey::KEY_W))
			camera.translate(camera.forward() * moveSpeed);

		if (inputManager.isKeyDown(EKey::KEY_S))
			camera.translate(camera.back() * moveSpeed);

		if (inputManager.isKeyDown(EKey::KEY_A))
			camera.translate(camera.left() * moveSpeed);

		if (inputManager.isKeyDown(EKey::KEY_D))
			camera.translate(camera.right() * moveSpeed);

		if (inputManager.isKeyDown(EKey::KEY_SPACE))
			camera.translate(Vector3::up() * moveSpeed);

		if (inputManager.isKeyDown(EKey::KEY_LEFT_SHIFT)
			|| inputManager.isKeyDown(EKey::KEY_RIGHT_SHIFT))
			camera.translate(Vector3::down() * moveSpeed);

		// Rotation
		if (inputManager.isKeyDown(EKey::KEY_UP))
			camera.rotate(ROTATION_SPEED * Vector3::right() * deltaTime);

		if (inputManager.isKeyDown(EKey::KEY_DOWN))
			camera.rotate(-ROTATION_SPEED * Vector3::right() * deltaTime);

		if (inputManager.isKeyDown(EKey::KEY_Q) ||
			inputManager.isKeyDown(EKey::KEY_LEFT))
			camera.rotate(-ROTATION_SPEED * Vector3::up() * deltaTime);

		if (inputManager.isKeyDown(EKey::KEY_E) ||
			inputManager.isKeyDown(EKey::KEY_RIGHT))
			camera.rotate(ROTATION_SPEED * Vector3::up() * deltaTime);

		m_spotLight.m_position = camera.getPosition();
		m_spotLight.m_direction = camera.forward();
	}

	void DemoApp::handleMouse() const
	{
		auto& inputManager = LGL_SERVICE(InputManager);
		auto& camera = Camera::getCurrent();

		if (!inputManager.isMouseButtonPressed(EMouseButton::MOUSE_BUTTON_LEFT) &&
			!inputManager.isMouseButtonPressed(EMouseButton::MOUSE_BUTTON_RIGHT))
		{
			LGL_SERVICE(Window).showCursor();
			inputManager.resetFirstMouse();
			return;
		}

		LGL_SERVICE(Window).disableCursor();

		//update camera rotation
		const float rotationSpeed = ROTATION_SPEED * MOUSE_SENSITIVITY
			* m_context->m_timer->getDeltaTime();

		const Vector2 mouseDelta = inputManager.getMouseDelta();

		if (mouseDelta == Vector2::zero())
			return;

		Vector3 camRotation = camera.getRotation();
		camRotation.m_x += -mouseDelta.m_y * rotationSpeed;
		camRotation.m_y += mouseDelta.m_x * rotationSpeed;

		camRotation.m_x = clamp(camRotation.m_x, -80.f, 80.f);
		camRotation.m_y = wrap(camRotation.m_y, 0.f, 360.f);

		camera.setRotation(camRotation);
	}

	void DemoApp::render()
	{
		const auto& renderer = LGL_SERVICE(Renderer);
		renderer.clear(Camera::getCurrent());

		updateLitShader("shaders/Lit.glsl");

		m_scene.update();
	}

	void DemoApp::updateLitShader(const std::string& fileName) const
	{
		const Shader* shader = setupLitShader(fileName);

		if (shader == nullptr)
			return;

		shader->use();

		// Setup camera
		shader->setUniformVec3("u_viewPos", Camera::getCurrent().getGlobalTransform().getPosition());

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
}

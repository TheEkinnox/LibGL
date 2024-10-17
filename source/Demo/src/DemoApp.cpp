#include "DemoApp.h"

#include "DemoContext.h"

#include <Angle.h>
#include <Arithmetic.h>
#include <BoxCollider.h>
#include <CapsuleCollider.h>
#include <InputManager.h>
#include <Raycast.h>
#include <Rigidbody.h>
#include <SphereCollider.h>
#include <Trigonometry.h>

#include <Debug/Assertion.h>
#include <Debug/Log.h>

#include <LowRenderer/Model.h>

#include <Resources/Mesh.h>
#include <Resources/MeshMulti.h>
#include <Resources/Shader.h>
#include <Resources/Texture.h>

#include <Utility/ServiceLocator.h>

#define MOVE_SPEED .5f
#define ROTATION_SPEED 30_deg
#define MOUSE_SENSITIVITY .8f
#define TEST_POOL 0

using namespace LibMath;
using namespace LibMath::Literal;
using namespace LibGL::Utility;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;
using namespace LibGL::Rendering::Resources;
using namespace LibGL::Physics;
using namespace LibGL::Application;

#define SHADOW_MAP_WIDTH 1024
#define SHADOW_MAP_HEIGHT SHADOW_MAP_WIDTH

namespace LibGL::Demo
{
    DemoApp::DemoApp(const int windowWidth, const int windowHeight, const char* title)
        : IApplication(std::make_unique<DemoContext>(windowWidth, windowHeight, title)), m_controllableModel(nullptr),
        m_shadowMap(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, ETextureFormat::DEPTH_COMPONENT),
        m_lightsSSBO(EAccessSpecifier::STREAM_DRAW)
    {
        // Initialize debugger
        Debug::Log::openFile("debug.log");

        m_frameBuffer.attach(m_shadowMap, EFrameBufferAttachment::DEPTH);
        m_lightsSSBO.bind(0);
    }

    void DemoApp::testThreadPool(const size_t taskCount, const size_t taskDuration)
    {
        DEBUG_LOG("Testing thread pool - Executing %llu tasks of %llums\r\n", taskCount, taskDuration);

        auto start = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < taskCount; ++i)
            std::this_thread::sleep_for(std::chrono::milliseconds(taskDuration));

        auto end = std::chrono::high_resolution_clock::now();
        DEBUG_LOG("Singe thread: %dms\r\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

        start = std::chrono::high_resolution_clock::now();
        std::vector<std::future<void>> tasks;
        tasks.reserve(taskCount);

        for (size_t i = 0; i < taskCount; ++i)
        {
            tasks.emplace_back(LGL_SERVICE(ThreadPool).enqueue([taskDuration]
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(taskDuration));
            }));
        }

        for (auto& task : tasks)
            task.wait();

        end = std::chrono::high_resolution_clock::now();
        DEBUG_LOG("Multi thread: %dms\r\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
    }

    void DemoApp::onStart()
    {
#if TEST_POOL
        testThreadPool(50, 200);
#endif

        loadResourcesMulti();
        createScene();
    }

    void DemoApp::onUpdate()
    {
        if (m_scene.isEmpty())
        {
            createScene();
            return;
        }

        // Handle keyboard and mouse inputs
        handleKeyboard();
        handleMouse();

        // Update scene
        m_scene.update();

        // Handle rendering
        render();

        // Swap render buffers
        LGL_SERVICE(Window).swapBuffers();
    }

    void DemoApp::loadResources()
    {
        DEBUG_LOG("Loading resources (single-thread)\r\n");
        auto& resourceManager = LGL_SERVICE(ResourceManager);

        const auto loadStart = std::chrono::high_resolution_clock::now();

        ASSERT(resourceManager.load<Mesh>("assets/meshes/primitives/plane.obj") != nullptr);
        ASSERT(resourceManager.load<Mesh>("assets/meshes/primitives/quad.obj") != nullptr);
        ASSERT(resourceManager.load<Mesh>("assets/meshes/primitives/sphere.obj") != nullptr);
        ASSERT(resourceManager.load<Mesh>("assets/meshes/primitives/cube.obj") != nullptr);
        ASSERT(resourceManager.load<Mesh>("assets/meshes/bunny.obj") != nullptr);

        ASSERT(resourceManager.load<Shader>("assets/shaders/Unlit.glsl") != nullptr);
        ASSERT(resourceManager.load<Shader>("assets/shaders/Normal.glsl") != nullptr);
        ASSERT(resourceManager.load<Shader>("assets/shaders/Basic.glsl") != nullptr);
        ASSERT(resourceManager.load<Shader>("assets/shaders/Lit.glsl") != nullptr);
        ASSERT(resourceManager.load<Shader>("assets/shaders/Split.glsl") != nullptr);
        ASSERT(resourceManager.load<Shader>("assets/shaders/Depth.glsl") != nullptr);
        ASSERT(resourceManager.load<Shader>("assets/shaders/DrawDepth.glsl") != nullptr);

        ASSERT(resourceManager.load<Texture>("assets/textures/container.jpg") != nullptr);
        ASSERT(resourceManager.load<Texture>("assets/textures/container2.png") != nullptr);
        ASSERT(resourceManager.load<Texture>("assets/textures/container2_specular.png") != nullptr);
        ASSERT(resourceManager.load<Texture>("assets/textures/grid.tga") != nullptr);

        const auto end = std::chrono::high_resolution_clock::now();
        DEBUG_LOG("Resources loaded in %dms\r\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - loadStart).count());
    }

    void DemoApp::loadResourcesMulti()
    {
        DEBUG_LOG("Loading resources (multi-thread)\r\n");
        auto& resourceManager = LGL_SERVICE(ResourceManager);

        std::vector<std::future<IResource*>> tasks;

        const auto loadStart = std::chrono::high_resolution_clock::now();

        tasks.emplace_back(resourceManager.loadInBackground<MeshMulti, IResource>("assets/meshes/primitives/plane.obj"));
        tasks.emplace_back(resourceManager.loadInBackground<MeshMulti, IResource>("assets/meshes/primitives/quad.obj"));
        tasks.emplace_back(resourceManager.loadInBackground<MeshMulti, IResource>("assets/meshes/primitives/sphere.obj"));
        tasks.emplace_back(resourceManager.loadInBackground<MeshMulti, IResource>("assets/meshes/primitives/cube.obj"));
        tasks.emplace_back(resourceManager.loadInBackground<MeshMulti, IResource>("assets/meshes/bunny.obj"));

        tasks.emplace_back(resourceManager.loadInBackground<Shader, IResource>("assets/shaders/Unlit.glsl"));
        tasks.emplace_back(resourceManager.loadInBackground<Shader, IResource>("assets/shaders/Normal.glsl"));
        tasks.emplace_back(resourceManager.loadInBackground<Shader, IResource>("assets/shaders/Basic.glsl"));
        tasks.emplace_back(resourceManager.loadInBackground<Shader, IResource>("assets/shaders/Lit.glsl"));
        tasks.emplace_back(resourceManager.loadInBackground<Shader, IResource>("assets/shaders/Split.glsl"));
        tasks.emplace_back(resourceManager.loadInBackground<Shader, IResource>("assets/shaders/Depth.glsl"));
        tasks.emplace_back(resourceManager.loadInBackground<Shader, IResource>("assets/shaders/DrawDepth.glsl"));

        tasks.emplace_back(resourceManager.loadInBackground<Texture, IResource>("assets/textures/container.jpg"));
        tasks.emplace_back(resourceManager.loadInBackground<Texture, IResource>("assets/textures/container2.png"));
        tasks.emplace_back(resourceManager.loadInBackground<Texture, IResource>("assets/textures/container2_specular.png"));
        tasks.emplace_back(resourceManager.loadInBackground<Texture, IResource>("assets/textures/grid.tga"));

        for (auto& task : tasks)
        {
            IResource* resource = task.get();
            ASSERT(resource != nullptr && resource->init());
        }

        const auto end = std::chrono::high_resolution_clock::now();
        DEBUG_LOG("Resources loaded in %dms with multithreading\r\n",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - loadStart).count());
    }

    void DemoApp::createScene()
    {
        m_scene.clear();

        auto& resourceManager = LGL_SERVICE(ResourceManager);
        auto& camera          = Camera::getCurrent();

        camera.setPosition({ 0.f, 1.8f, 0.f });
        camera.setRotation(Quaternion::identity());

        //const Shader* shader = resourceManager.get<Shader>("assets/shaders/Unlit.glsl");
        //const Shader* shader = resourceManager.get<Shader>("assets/shaders/Normal.glsl");
        //const Shader* shader = resourceManager.get<Shader>("assets/shaders/Basic.glsl");
        Shader* shader = resourceManager.get<Shader>("assets/shaders/Split.glsl");
        ASSERT(shader != nullptr);

        // Load the models
        const Mesh* floorMesh = resourceManager.get<Mesh>("assets/meshes/primitives/plane.obj");
        ASSERT(floorMesh != nullptr);

        const Texture* floorDiffuse = resourceManager.get<Texture>("assets/textures/container2.png");
        ASSERT(floorDiffuse != nullptr);

        const Texture* floorSpecular = resourceManager.get<Texture>("assets/textures/container2_specular.png");
        ASSERT(floorSpecular != nullptr);

        const Mesh* sphereMesh = resourceManager.get<Mesh>("assets/meshes/primitives/sphere.obj");
        ASSERT(sphereMesh != nullptr);

        const Texture* sphereDiffuse = resourceManager.get<Texture>("assets/textures/grid.tga");
        ASSERT(sphereDiffuse != nullptr);

        const Mesh* bunnyMesh = resourceManager.get<Mesh>("assets/meshes/bunny.obj");
        ASSERT(bunnyMesh != nullptr);

        const Mesh* cubeMesh = resourceManager.get<Mesh>("assets/meshes/primitives/cube.obj");
        ASSERT(cubeMesh != nullptr);

        const Texture* cubeDiffuse = resourceManager.get<Texture>("assets/textures/container.jpg");
        ASSERT(cubeDiffuse != nullptr);

        const Material floorMat(
            *shader,
            { floorDiffuse, floorSpecular, nullptr },
            { Vector2(.5f), Vector2(14.f) },
            { Color(.25f, .6f, .9f) },
            32.f
        );

        const Material sphereMat(
            *shader,
            { sphereDiffuse, nullptr, nullptr },
            { Vector2::zero(), Vector2::one() },
            { { .52f, .32f, .22f } },
            8.f
        );

        const Material bunnyMat(
            *shader,
            { nullptr, nullptr, nullptr },
            { Vector2::zero(), Vector2::one() },
            { Color::gray },
            16.f
        );

        const Material cubeMat(
            *shader,
            { cubeDiffuse, nullptr, nullptr },
            { Vector2::zero(), Vector2::one() },
            { Color::white },
            16.f
        );

        // Place the models
        Model& floorModel = m_scene.addNode<Model>(nullptr, *floorMesh, floorMat);
        floorModel.setScale(Vector3(14.f, 1.f, 14.f));
        floorModel.addComponent<BoxCollider>(Vector3(0.f, -.05f, 0.f), Vector3(1.f, .1f, 1.f));

        Model& bunnyModel = m_scene.addNode<Model>(nullptr, *bunnyMesh, bunnyMat);
        bunnyModel.setPosition(Vector3(0.f, 1.f, -3.f));
        bunnyModel.setScale(Vector3(.5f));
        bunnyModel.addComponent<CapsuleCollider>(Vector3(.25f, .65f, .2f), Vector3::right(), 3.2f, 1.f);
        bunnyModel.addComponent<Rigidbody>();

        m_controllableModel = &bunnyModel;

        Model& cubeModel = m_scene.addNode<Model>(nullptr, *cubeMesh, cubeMat);
        cubeModel.setPosition(Vector3(0.f, 3.f, -4.5f));
        cubeModel.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
        cubeModel.addComponent<Rigidbody>();

        Model& leftSphereModel = bunnyModel.addChild<Model>(nullptr, *sphereMesh, sphereMat);
        leftSphereModel.setPosition(Vector3(-3.f, 4.f, -1.5f));
        leftSphereModel.addComponent<SphereCollider>(Vector3::zero(), 1.f);

        Model& rightSphereModel = bunnyModel.addChild<Model>(nullptr, *sphereMesh, sphereMat);
        rightSphereModel.setPosition(Vector3(3.f, 4.f, -1.5f));
        rightSphereModel.addComponent<SphereCollider>(Vector3::zero(), 1.f);

        // Setup the lights
        m_dirLight =
        {
            Color(1.f, .94f, .91f, 1.f),
            Vector3(.6f, -.7f, -.4f)
        };

        m_spotLight =
        {
            Color(1, 1, 1, 8),
            camera.getWorldPosition(),
            camera.worldBack(),
            AttenuationData(10),
            {
                cos(0_deg),
                cos(30_deg)
            }
        };

        m_lightMatrices.clear();

        m_lightMatrices.emplace_back(Light(Color(.2f, .2f, .2f, 1.f)).getMatrix());
        m_lightMatrices.emplace_back(m_dirLight.getMatrix());
        m_lightMatrices.emplace_back(m_spotLight.getMatrix());

        m_lightMatrices.emplace_back(PointLight(Light{ Color::magenta }, Vector3{ -1, 1, 1 }, AttenuationData(16)).getMatrix());
        m_lightMatrices.emplace_back(PointLight(Light{ Color::magenta }, Vector3{ 1, 1, 1 }, AttenuationData(16)).getMatrix());
        m_lightMatrices.emplace_back(PointLight(Light{ Color::magenta }, Vector3{ 0, 1, -1 }, AttenuationData(16)).getMatrix());
    }

    void DemoApp::handleKeyboard()
    {
        const auto& inputManager = LGL_SERVICE(InputManager);
        Camera&     camera       = Camera::getCurrent();

        if (inputManager.isKeyDown(EKey::KEY_ESCAPE))
            LGL_SERVICE(Window).setShouldClose(true);

        if (inputManager.isKeyPressed(EKey::KEY_R))
        {
            createScene();
            return;
        }

        const bool isShiftDown = inputManager.isKeyDown(EKey::KEY_LEFT_SHIFT) || inputManager.isKeyDown(EKey::KEY_RIGHT_SHIFT);

        if (inputManager.isKeyPressed(EKey::KEY_L))
        {
            if (isShiftDown)
                loadResources();
            else
                loadResourcesMulti();

            m_scene.clear();
            return;
        }

        if (inputManager.isKeyPressed(EKey::KEY_X))
        {
            for (const auto& node : m_scene.getNodes())
            {
                if (isShiftDown)
                    node->removeChildren<Model>();
                else
                    node->removeChild<Model>();
            }
        }

        const float deltaTime = LGL_SERVICE(Timer).getDeltaTime();

        // Dir light rotation
        if (inputManager.isKeyDown(EKey::KEY_U))
            m_dirLight.m_direction.rotate(Degree(ROTATION_SPEED * deltaTime), Vector3::right());

        if (inputManager.isKeyDown(EKey::KEY_H))
            m_dirLight.m_direction.rotate(Degree(-ROTATION_SPEED * deltaTime), Vector3::up());

        if (inputManager.isKeyDown(EKey::KEY_J))
            m_dirLight.m_direction.rotate(Degree(-ROTATION_SPEED * deltaTime), Vector3::right());

        if (inputManager.isKeyDown(EKey::KEY_K))
            m_dirLight.m_direction.rotate(Degree(ROTATION_SPEED * deltaTime), Vector3::up());

        static const Matrix4 lightProjection = orthographicProjection(-10.f, 10.f, -10.f, 10.f, .01f, 20.f);
        static const Matrix4 frontToUpMat    = rotationFromTo(Vector3::front(), Vector3::up());

        const Vector3 lightUp   = (frontToUpMat * Vector4(m_dirLight.m_direction, 0.f)).xyz();
        const Matrix4 lightView = lookAt(m_dirLight.m_direction * -8.f, Vector3::zero(), lightUp);
        m_lightViewProjection   = lightProjection * lightView;

        // Movement
        float moveSpeed = MOVE_SPEED;

        if (inputManager.isKeyDown(EKey::KEY_LEFT_CONTROL) || inputManager.isKeyDown(EKey::KEY_RIGHT_CONTROL))
            moveSpeed *= 1.5f;

        const bool shouldControlModel = m_controllableModel && inputManager.isKeyDown(EKey::KEY_LEFT_ALT);
        Entity*    target = shouldControlModel ? static_cast<Entity*>(m_controllableModel) : static_cast<Entity*>(&camera);

        TVector3<Degree> angles = target->getEuler(ERotationOrder::YXZ);

        // Rotation
        if (inputManager.isKeyDown(EKey::KEY_UP))
            angles.m_x += ROTATION_SPEED * deltaTime;

        if (inputManager.isKeyDown(EKey::KEY_DOWN))
            angles.m_x -= ROTATION_SPEED * deltaTime;

        if (inputManager.isKeyDown(EKey::KEY_Q) || inputManager.isKeyDown(EKey::KEY_LEFT))
            angles.m_y += ROTATION_SPEED * deltaTime;

        if (inputManager.isKeyDown(EKey::KEY_E) || inputManager.isKeyDown(EKey::KEY_RIGHT))
            angles.m_y -= ROTATION_SPEED * deltaTime;

        angles.m_x = clamp(angles.m_x, -85_deg, 85_deg);
        target->setEuler(angles, ERotationOrder::YXZ);

        Vector3 direction;

        if (inputManager.isKeyDown(EKey::KEY_W))
            direction += target->back();

        if (inputManager.isKeyDown(EKey::KEY_S))
            direction += target->forward();

        if (inputManager.isKeyDown(EKey::KEY_A))
            direction += target->left();

        if (inputManager.isKeyDown(EKey::KEY_D))
            direction += target->right();

        if (direction != Vector3::zero())
            direction.normalize();

        if (shouldControlModel)
        {
            if (Rigidbody* rb = m_controllableModel->getComponent<Rigidbody>())
            {
                if (inputManager.isKeyPressed(EKey::KEY_SPACE))
                    rb->addForce(Vector3::up() * 4.f, EForceMode::IMPULSE);

                Vector3 targetVelocity;

                if (!floatEquals(moveSpeed, 0.f) && direction != Vector3::zero())
                    targetVelocity = direction * (moveSpeed / direction.magnitude());

                targetVelocity.m_y += rb->m_velocity.m_y;
                rb->m_velocity = targetVelocity;
            }
        }
        else
        {
            moveSpeed *= deltaTime;

            camera.translate(direction * moveSpeed);

            if (inputManager.isKeyDown(EKey::KEY_SPACE))
                camera.translate(Vector3::up() * moveSpeed);

            if (inputManager.isKeyDown(EKey::KEY_LEFT_SHIFT) || inputManager.isKeyDown(EKey::KEY_RIGHT_SHIFT))
                camera.translate(Vector3::down() * moveSpeed);

            m_spotLight.m_position  = camera.getWorldPosition();
            m_spotLight.m_direction = camera.worldBack();
        }

        if (inputManager.isKeyPressed(EKey::KEY_C))
        {
            RaycastHit  hitInfo;
            const auto& camCollider = camera.getComponent<ICollider>();
            Vector3     castOffset  = Vector3::zero();

            if (camCollider != nullptr)
                castOffset = camera.forward() * (camCollider->getBounds().m_sphereRadius + .01f);

            if (raycast(camera.getPosition() + castOffset, camera.forward(), hitInfo))
                hitInfo.m_collider->getOwner().translate(Vector3::down());
        }
    }

    void DemoApp::handleMouse() const
    {
        auto& inputManager = LGL_SERVICE(InputManager);

        if (!inputManager.isMouseButtonDown(EMouseButton::MOUSE_BUTTON_LEFT) &&
            !inputManager.isMouseButtonDown(EMouseButton::MOUSE_BUTTON_RIGHT))
        {
            LGL_SERVICE(Window).showCursor();
            inputManager.resetFirstMouse();
            return;
        }

        LGL_SERVICE(Window).disableCursor();

        //update camera rotation
        const Radian  rotationSpeed = ROTATION_SPEED * MOUSE_SENSITIVITY * m_context->m_timer->getDeltaTime();
        const Vector2 mouseDelta    = inputManager.getMouseDelta();

        if (rotationSpeed == 0_deg || mouseDelta == Vector2::zero())
            return;

        Camera&          camera = Camera::getCurrent();
        TVector3<Degree> angles = camera.getEuler(ERotationOrder::YXZ);

        angles.m_y -= rotationSpeed * mouseDelta.m_x;
        angles.m_y.wrap();

        angles.m_x -= rotationSpeed * mouseDelta.m_y;
        angles.m_x = clamp(angles.m_x, -90_deg, 90_deg);

        camera.setEuler(angles, ERotationOrder::YXZ);
    }

    void DemoApp::render()
    {
        auto& sceneRenderer = LGL_SERVICE(SceneRenderer);
        sceneRenderer.init(m_scene);

        const auto& renderer = LGL_SERVICE(Renderer);
        renderer.setViewPort(0, 0, m_shadowMap.getWidth(), m_shadowMap.getHeight());

        m_frameBuffer.bind();

        renderer.clear(false, true, false);

        const ResourceManager& resourceManager = LGL_SERVICE(ResourceManager);
        Shader*                shadowMapShader = resourceManager.get<Shader>("assets/shaders/Depth.glsl");
        sceneRenderer.render(m_lightViewProjection, shadowMapShader);

        m_frameBuffer.unbind();

        const Vector2I windowSize = m_context->m_window->getSize();
        renderer.setViewPort(0, 0, windowSize.m_x, windowSize.m_y);

        const Camera& cam = Camera::getCurrent();
        renderer.clear(cam);

        m_lightMatrices[1] = m_dirLight.getMatrix();
        m_lightMatrices[2] = m_spotLight.getMatrix();

        m_lightsSSBO.setData(m_lightMatrices.data(), m_lightMatrices.size());
        updateLitShader("assets/shaders/Split.glsl");
        sceneRenderer.render(cam.getViewProjectionMatrix(), nullptr);
    }

    void DemoApp::updateLitShader(const std::string& fileName) const
    {
        Shader* shader = LGL_SERVICE(ResourceManager).get<Shader>(fileName);

        if (shader == nullptr)
            return;

        shader->use();

        // Setup camera
        shader->setUniformVec3("u_viewPos", Camera::getCurrent().getWorldPosition());

        // Setup light space matrix
        shader->setUniformMat4("u_lightSpaceMat", m_lightViewProjection);

        // Setup shadow map
        m_shadowMap.bind(4);
        shader->setUniformInt("u_shadowMap", 4);

        Shader::unbind();
    }
}

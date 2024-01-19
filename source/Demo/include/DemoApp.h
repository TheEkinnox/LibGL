#pragma once
#include <Core/Buffers/FrameBuffer.h>
#include <Core/Buffers/ShaderStorageBuffer.h>

#include <Resources/Texture.h>

#include "Scene.h"
#include "LowRenderer/Camera.h"
#include "LowRenderer/Light.h"
#include "IApplication.h"

namespace LibGL::Rendering
{
    class Model;
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
        Resources::Scene                m_scene;
        std::vector<LibMath::Matrix4x4> m_lightMatrices;
        Rendering::DirectionalLight     m_dirLight;
        Rendering::SpotLight            m_spotLight;
        Rendering::Model*               m_controllableModel;

        Rendering::Resources::Texture m_shadowMap;
        Rendering::FrameBuffer        m_frameBuffer;
        LibMath::Matrix4x4            m_lightViewProjection;

        Rendering::ShaderStorageBuffer m_lightsSSBO;

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
        void onStop() override
        {
        }

        /**
         * \brief Executes the given number of tasks of the given duration with and without the thread pool
         * \param taskCount The number of tasks to execute
         * \param taskDuration The duration of the task in ms
         */
        static void testThreadPool(size_t taskCount, size_t taskDuration);

        /**
         * \brief Loads the necessary resources for the scene
         */
        static void loadResources();

        /**
         * \brief Loads the necessary resources for the scene using multithreading
         */
        static void loadResourcesMulti();

        /**
         * \brief Creates the 3d environment
         */
        void createScene();

        /**
         * \brief Loads or creates a new shader from the given file
         */
        static Rendering::Resources::Shader* setupShader(const std::string& fileName);

        /**
         * \brief Initializes the given shader
         * \param shader The shader to initialize
         */
        static void initShader(Rendering::Resources::Shader* shader);

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

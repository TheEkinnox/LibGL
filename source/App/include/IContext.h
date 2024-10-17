#pragma once
#include "Window.h"
#include "Utility/Timer.h"

#include <memory>

namespace LibGL::Application
{
    class IContext
    {
    public:
        std::unique_ptr<Window>         m_window;
        std::unique_ptr<Utility::Timer> m_timer;

        /**
         * \brief Creates a GLFW window and initializes OpenGL with GLAD
         * \param windowWidth The created window's width
         * \param windowHeight The created window's height
         * \param title The created window's title
         */
        IContext(int windowWidth, int windowHeight, const char* title);

        /**
         * \brief Disable the context's copy constructor
         */
        IContext(const IContext&) = delete;

        /**
         * \brief Disable the context's move constructor
         */
        IContext(IContext&&) = delete;

        /**
         * \brief Clears the context's previously allocated resources
         */
        virtual ~IContext();

        /**
         * \brief Disable the context's copy assignment operator
         */
        IContext& operator=(const IContext&) = delete;

        /**
         * \brief Disable the context's move assignment operator
         */
        IContext& operator=(IContext&&) = delete;

        /**
         * \brief The function to call to update the context's data
         */
        virtual void update();

    protected:
        static void bindDebugCallback();
    };
}

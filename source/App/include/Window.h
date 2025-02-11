#pragma once
#include "EInputModifier.h"
#include "EKey.h"
#include "EKeyState.h"
#include "EMouseButton.h"
#include "EMouseButtonState.h"
#include "Eventing/Event.h"
#include "Vector/Vector2.h"

#include <stdexcept>
#include <unordered_map>

// Forward declaration of GLFWwindow to avoid including glfw in the header
using GLFWwindow = struct GLFWwindow;

namespace LibGL::Application
{
    class Window
    {
    public:
        using dimensions_t = LibMath::Vector2I;
        using pos_t = LibMath::Vector2I;

        Event<EKey, int, EKeyState, EInputModifier>            m_keyEvent;
        Event<EMouseButton, EMouseButtonState, EInputModifier> m_mouseButtonEvent;
        Event<dimensions_t>                                    m_resizeEvent;

        /**
         * \brief Creates a GLFW window
         * \param size The created window's size
         * \param title The created window's title
         */
        Window(dimensions_t size, const char* title);

        /**
         * \brief Disable the Application's copy constructor
         */
        Window(const Window&) = delete;

        /**
         * \brief Disable the Application's move constructor
         */
        Window(Window&&) = delete;

        /**
         * \brief Clears the application's previously allocated resources
         */
        ~Window() = default;

        /**
         * \brief Disable the Application's copy assignment operator
         */
        Window& operator=(const Window&) = delete;

        /**
         * \brief Disable the Application's move assignment operator
         */
        Window& operator=(Window&&) = delete;

        /**
         * \brief Sets the window as glfw's current context
         */
        void makeCurrentContext() const;

        /**
         * \brief Gets the window's title
         * \return The window's title
         */
        std::string getTitle() const;

        /**
         * \brief Gets the window's current position
         * \return The window's current position
         */
        pos_t getPosition() const;

        /**
         * \brief Gets the window's current size
         * \return The window's current size
         */
        dimensions_t getSize() const;

        /**
         * \brief Sets the window's current size
         */
        void setSize(dimensions_t size);

        /**
         * \brief Gets the window's minimum size
         * \return The window's minimum size
         */
        dimensions_t getMinSize() const;

        /**
         * \brief Gets the window's maximum size
         * \return The window's maximum size
         */
        dimensions_t getMaxSize() const;

        /**
         * \brief Gets the current cursor position
         * \return The current cursor position
         */
        LibMath::Vector2 getCursorPosition() const;

        /**
         * \brief Shows the mouse cursor
         */
        void showCursor() const;

        /**
         * \brief Hides the mouse cursor
         */
        void hideCursor() const;

        /**
         * \brief Hides the cursor and locks it to the window
         */
        void disableCursor() const;

        /**
         * \brief Whether the window should close or not
         * \return True if the window should close. False otherwise.
         */
        bool shouldClose() const;

        /**
         * \brief Swaps the render buffers
         */
        void swapBuffers() const;

        /**
         * \brief Sets whether the window should close or not
         * \param shouldClose Whether the window should close or not
         */
        void setShouldClose(bool shouldClose) const;

        /**
         * \brief Gets the window's aspect ratio
         * \return The window's aspect ratio
         */
        float getAspect() const;

    private:
        inline static std::unordered_map<GLFWwindow*, Window*> s_windowsMap;

        std::string  m_title;
        dimensions_t m_size;
        dimensions_t m_minSize;
        dimensions_t m_maxSize;
        pos_t        m_pos;

        GLFWwindow* m_glfwWindow;

        int  m_refreshRate;
        bool m_isFullScreen;

        /**
         * \brief Finds the LibGL window linked to the given GLFW window
         * \param window The glfw window linked to the LibGL window to find.
         * \return A pointer to the found window or nullptr if no window was found.
         */
        static Window* getInstance(GLFWwindow* window);

        /**
         * \brief GLFW frame buffer size change callback
         * \param glfwWindow The window for which the frame buffer size has changed
         * \param width The window's frame buffer's new width
         * \param height The window's frame buffer's new height
         */
        static void onFrameBufferResize(GLFWwindow* glfwWindow, int width, int height);

        /**
         * \brief GLFW key action callback
         * \param glfwWindow The window for which a key action has been performed
         * \param key The key that has been interacted with
         * \param scanCode The scan code of the key that has been interacted with
         * \param action The action that has been performed
         * \param mods The input modifiers of the key event
         */
        static void onKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int mods);

        /**
         * \brief GLFW mouse button action callback
         * \param glfwWindow The window for which a mouse button has been interacted with
         * \param button The mouse button that has been interacted with
         * \param action The action that has been performed
         * \param mods The input modifiers of the key event
         */
        static void onMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods);
    };
}

#pragma once
#include <functional>
#include <stdexcept>
#include <unordered_map>

#include "EInputModifier.h"
#include "EKey.h"
#include "EKeyState.h"
#include "EMouseButton.h"
#include "EMouseButtonState.h"
#include "Vector/Vector2.h"

// Forward declaration of GLFWwindow to avoid including glfw in the header
typedef struct GLFWwindow GLFWwindow;

namespace LibGL::Application
{
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
	}

	class Window
	{
	public:
		using dimensions_t = std::pair<int, int>;
		using pos_t = std::pair<int, int>;
		using key_callback_t = std::function<void(EKey key, int scanCode, EKeyState action, EInputModifier mods)>;
		using mouse_callback_t = std::function<void(EMouseButton button, EMouseButtonState action, EInputModifier mods)>;

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
		 * \brief Registers the given keyboard input callback and returns it's id
		 * \param callback The keyboard input callback to register
		 * \return The id of the added callback
		 */
		uint64_t addKeyCallback(const key_callback_t& callback);

		/**
		 * \brief Removes the key callback with the given id.
		 * \param callbackId The id of the callback to remove.
		 */
		void removeKeyCallback(uint64_t callbackId);

		/**
		 * \brief Registers the given mouse button callback and returns it's id
		 * \param callback The mouse button callback to register
		 * \return The id of the added callback
		 */
		uint64_t addMouseButtonCallback(const mouse_callback_t& callback);

		/**
		 * \brief Removes the mouse button callback with the given id.
		 * \param callbackId The id of the callback to remove.
		 */
		void removeMouseButtonCallback(uint64_t callbackId);

		/**
		 * \brief Swaps the render buffers
		 */
		void swapBuffers() const;

		/**
		 * \brief Sets whether the window should close or not
		 * \param shouldClose Whether the window should close or not
		 */
		void setShouldClose(bool shouldClose) const;

	private:
		inline static std::unordered_map<GLFWwindow*, Window*> s_windowsMap;

		std::unordered_map<uint64_t, key_callback_t>	m_keyCallbacks;
		std::unordered_map<uint64_t, mouse_callback_t>	m_mouseCallbacks;

		uint64_t			m_currentKeyCallbackId = 1;
		uint64_t			m_currentMouseCallbackId = 1;

		std::string			m_title;
		dimensions_t		m_size;
		dimensions_t		m_minSize;
		dimensions_t		m_maxSize;
		pos_t				m_pos;

		GLFWwindow*			m_glfwWindow;

		int					m_refreshRate;
		bool				m_isFullScreen;

		/**
		 * \brief Finds the LibGL window linked to the given GLFW window
		 * \param window The glfw window linked to the LibGL window to find.
		 * \return A pointer to the found window or nullptr if no window was found.
		 */
		static Window* getInstance(GLFWwindow* window);

		/**
		 * \brief OpenGL frame buffer size change callback
		 * \param glfwWindow The window for which the frame buffer size has hanged
		 * \param width The window's frame buffer's new width
		 * \param height The window's frame buffer's new height
		 */
		static void onFrameBufferResize(GLFWwindow* glfwWindow, int width, int height);

		/**
		 * \brief OpenGL key action callback
		 * \param glfwWindow The window for which a key action has been performed
		 * \param key The key that has been interacted with
		 * \param scanCode The scan code of the key that has been interacted with
		 * \param action The action that has been performed
		 * \param mods The input modifiers of the key event
		 */
		static void onKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int mods);

		/**
		 * \brief OpenGL frame buffer size change callback
		 * \param glfwWindow The window for which the frame buffer size has hanged
		 * \param button The mouse button that has been interacted with
		 * \param action The action that has been performed
		 * \param mods The input modifiers of the key event
		 */
		static void onMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods);
	};
}

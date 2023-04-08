#include "Window.h"
#include <GLFW/glfw3.h>
#include <ranges>

#include "Debug/Log.h"

using namespace LibGL::Application::Exceptions;

namespace LibGL::Application
{
	Window::Window(const dimensions_t size, const char* title) :
		m_title(title), m_size(size), m_minSize(0, 0),
		m_maxSize(INT_MAX, INT_MAX), m_refreshRate(GLFW_DONT_CARE), m_isFullScreen(false)
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

		// Create glfw window
		m_glfwWindow = glfwCreateWindow(size.first, size.second, title,
			nullptr, nullptr);

		if (m_glfwWindow == nullptr)
		{
			DEBUG_LOG("Failed to create GLFW window\n");
			throw WindowCreationFailed("Failed to create GLFW window");
		}

		glfwGetWindowPos(m_glfwWindow, &m_pos.first, &m_pos.second);

		s_windowsMap[m_glfwWindow] = this;

		glfwSetFramebufferSizeCallback(m_glfwWindow, onFrameBufferResize);
		glfwSetKeyCallback(m_glfwWindow, onKey);
		glfwSetMouseButtonCallback(m_glfwWindow, onMouseButton);
	}

	void Window::makeCurrentContext() const
	{
		glfwMakeContextCurrent(m_glfwWindow);
	}

	std::string Window::getTitle() const
	{
		return m_title;
	}

	Window::pos_t Window::getPosition() const
	{
		return m_pos;
	}

	Window::dimensions_t Window::getSize() const
	{
		return m_size;
	}

	void Window::setSize(const dimensions_t size)
	{
		m_size = size;
	}

	Window::dimensions_t Window::getMinSize() const
	{
		return m_minSize;
	}

	Window::dimensions_t Window::getMaxSize() const
	{
		return m_maxSize;
	}

	bool Window::shouldClose() const
	{
		return glfwWindowShouldClose(m_glfwWindow);
	}

	uint64_t Window::addKeyCallback(const key_callback_t& callback)
	{
		m_keyCallbacks.emplace(m_currentKeyCallbackId, callback);
		return m_currentKeyCallbackId++;
	}

	void Window::removeKeyCallback(const uint64_t callbackId)
	{
		if (m_keyCallbacks.contains(callbackId))
			m_keyCallbacks.erase(callbackId);
	}

	uint64_t Window::addMouseButtonCallback(const mouse_callback_t& callback)
	{
		m_mouseCallbacks.emplace(m_currentMouseCallbackId, callback);
		return m_currentMouseCallbackId++;
	}

	void Window::removeMouseButtonCallback(const uint64_t callbackId)
	{
		if (m_mouseCallbacks.contains(callbackId))
			m_mouseCallbacks.erase(callbackId);
	}

	void Window::swapBuffers() const
	{
		glfwSwapBuffers(m_glfwWindow);
	}

	void Window::setShouldClose(const bool shouldClose) const
	{
		glfwSetWindowShouldClose(m_glfwWindow, shouldClose);
	}

	void Window::showCursor() const
	{
		glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::hideCursor() const
	{
		glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Window::disableCursor() const
	{
		glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	LibMath::Vector2 Window::getCursorPosition() const
	{
		double mouseX, mouseY;
		glfwGetCursorPos(m_glfwWindow, &mouseX, &mouseY);

		return LibMath::Vector2
		{
			static_cast<float>(mouseX),
			static_cast<float>(mouseY)
		};
	}

	Window* Window::getInstance(GLFWwindow* window)
	{
		return s_windowsMap.contains(window) ? s_windowsMap[window] : nullptr;
	}

	void Window::onFrameBufferResize(GLFWwindow* glfwWindow,
		const int width, const int height)
	{
		if (Window* window = getInstance(glfwWindow))
			window->setSize(dimensions_t(width, height));
	}

	void Window::onKey(GLFWwindow* glfwWindow, const int key, const int scanCode,
		const int action, const int mods)
	{
		if (const Window* window = getInstance(glfwWindow))
		{
			for (const auto& callback : window->m_keyCallbacks | std::views::values)
			{
				callback(static_cast<EKey>(key), scanCode,
					static_cast<EKeyState>(action), static_cast<EInputModifier>(mods));
			}
		}
	}

	void Window::onMouseButton(GLFWwindow* glfwWindow, const int button, const int action, const int mods)
	{
		if (const Window* window = getInstance(glfwWindow))
		{
			for (const auto& callback : window->m_mouseCallbacks | std::views::values)
			{
				callback(static_cast<EMouseButton>(button),
					static_cast<EMouseButtonState>(action),
					static_cast<EInputModifier>(mods));
			}
		}
	}
}

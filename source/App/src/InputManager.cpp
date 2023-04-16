#include "InputManager.h"

#include <GLFW/glfw3.h>
#include <functional>

#include "EKeyState.h"
#include "Window.h"

using namespace LibGL::Application::Exceptions;

namespace LibGL::Application
{
	InputManager::InputManager(Window& window) :
		m_window(window), m_isFirstMouse(true)
	{
		using namespace std::placeholders;

		m_keyCallbackId = m_window.m_keyEvent.subscribe(std::bind(&InputManager::keyCallback, this, _1, _2, _3, _4));
		m_mouseButtonCallbackId = m_window.m_mouseButtonEvent.subscribe(std::bind(&InputManager::mouseButtonCallback, this, _1, _2, _3));
	}

	InputManager::~InputManager()
	{
		m_window.m_keyEvent.unsubscribe(m_keyCallbackId);
		m_window.m_mouseButtonEvent.unsubscribe(m_mouseButtonCallbackId);
	}

	bool InputManager::isKeyDown(const EKey key) const
	{
		return m_keyStates.contains(key)
			&& (m_keyStates.at(key) == EKeyState::KEY_PRESSED
				|| m_keyStates.at(key) == EKeyState::KEY_REPEATED);
	}

	bool InputManager::isKeyDown(const int scanCode) const
	{
		return m_scanCodeStates.contains(scanCode)
			&& (m_scanCodeStates.at(scanCode) == EKeyState::KEY_PRESSED
				|| m_scanCodeStates.at(scanCode) == EKeyState::KEY_REPEATED);
	}

	bool InputManager::isKeyUp(const EKey key) const
	{
		return m_keyStates.contains(key)
			&& m_keyStates.at(key) == EKeyState::KEY_RELEASED;
	}

	bool InputManager::isKeyUp(const int scanCode) const
	{
		return m_scanCodeStates.contains(scanCode)
			&& m_scanCodeStates.at(scanCode) == EKeyState::KEY_RELEASED;
	}

	bool InputManager::isMouseButtonPressed(const EMouseButton button) const
	{
		return m_mouseButtonStates.contains(button)
			&& m_mouseButtonStates.at(button) == EMouseButtonState::MOUSE_PRESSED;
	}

	bool InputManager::isMouseButtonReleased(const EMouseButton button) const
	{
		return m_mouseButtonStates.contains(button)
			&& m_mouseButtonStates.at(button) == EMouseButtonState::MOUSE_RELEASED;
	}

	void InputManager::clearStates()
	{
		m_keyStates.clear();
		m_scanCodeStates.clear();
		m_mouseButtonStates.clear();
	}

	void InputManager::updateMouse()
	{
		m_mousePos = m_window.getCursorPosition();

		if (m_isFirstMouse)
		{
			m_lastMousePos = m_mousePos;
			m_isFirstMouse = false;
		}

		m_mouseDelta = m_mousePos - m_lastMousePos;
		m_lastMousePos = m_mousePos;
	}

	void InputManager::resetFirstMouse()
	{
		m_isFirstMouse = false;
	}

	LibMath::Vector2 InputManager::getMousePosition() const
	{
		return m_mousePos;
	}

	LibMath::Vector2 InputManager::getMouseDelta() const
	{
		return m_mouseDelta;
	}

	int InputManager::getScanCode(const EKey key)
	{
		return glfwGetKeyScancode(static_cast<int>(key));
	}

	void InputManager::keyCallback(const EKey key, const int scanCode, const EKeyState state, EInputModifier)
	{
		m_keyStates[key] = state;
		m_scanCodeStates[scanCode] = state;
	}

	void InputManager::mouseButtonCallback(const EMouseButton button, const EMouseButtonState state, EInputModifier)
	{
		m_mouseButtonStates[button] = state;
	}
}

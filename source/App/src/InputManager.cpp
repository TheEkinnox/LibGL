#include "InputManager.h"

#include <GLFW/glfw3.h>
#include <functional>

#include "EKeyState.h"
#include "Window.h"
#include "Utility/ServiceLocator.h"
#include "Utility/Timer.h"

using namespace LibGL::Application::Exceptions;
using namespace LibGL::Utility;

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

	bool InputManager::isKeyUp(const EKey key) const
	{
		return m_keyInfos.contains(key)
			&& m_keyInfos.at(key).m_keyState == EKeyState::KEY_RELEASED;
	}

	bool InputManager::isKeyUp(const int scanCode) const
	{
		return m_scanCodeInfo.contains(scanCode)
			&& m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_RELEASED;
	}

	bool InputManager::isKeyDown(const EKey key) const
	{
		return m_keyInfos.contains(key)
			&& (m_keyInfos.at(key).m_keyState == EKeyState::KEY_PRESSED
				|| m_keyInfos.at(key).m_keyState == EKeyState::KEY_REPEATED);
	}

	bool InputManager::isKeyDown(const int scanCode) const
	{
		return m_scanCodeInfo.contains(scanCode)
			&& (m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_PRESSED
				|| m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_REPEATED);
	}

	bool InputManager::isKeyPressed(const EKey key) const
	{
		return m_keyInfos.contains(key)
			&& m_keyInfos.at(key).m_keyState == EKeyState::KEY_PRESSED
			&& m_keyInfos.at(key).m_stateChangeFrame == LGL_SERVICE(Timer).getFrameCount();
	}

	bool InputManager::isKeyPressed(const int scanCode) const
	{
		return m_scanCodeInfo.contains(scanCode)
			&& m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_PRESSED
			&& m_scanCodeInfo.at(scanCode).m_stateChangeFrame == LGL_SERVICE(Timer).getFrameCount();
	}

	bool InputManager::isKeyReleased(const EKey key) const
	{
		return m_keyInfos.contains(key)
			&& m_keyInfos.at(key).m_keyState == EKeyState::KEY_RELEASED
			&& m_keyInfos.at(key).m_stateChangeFrame == LGL_SERVICE(Timer).getFrameCount();
	}

	bool InputManager::isKeyReleased(const int scanCode) const
	{
		return m_scanCodeInfo.contains(scanCode)
			&& m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_RELEASED
			&& m_scanCodeInfo.at(scanCode).m_stateChangeFrame == LGL_SERVICE(Timer).getFrameCount();
	}

	bool InputManager::isMouseButtonUp(const EMouseButton button) const
	{
		return m_mouseButtonInfo.contains(button)
			&& m_mouseButtonInfo.at(button).m_buttonState == EMouseButtonState::MOUSE_RELEASED;
	}

	bool InputManager::isMouseButtonDown(const EMouseButton button) const
	{
		return m_mouseButtonInfo.contains(button)
			&& m_mouseButtonInfo.at(button).m_buttonState == EMouseButtonState::MOUSE_PRESSED;
	}

	bool InputManager::isMouseButtonPressed(const EMouseButton button) const
	{
		return m_mouseButtonInfo.contains(button)
			&& m_mouseButtonInfo.at(button).m_buttonState == EMouseButtonState::MOUSE_PRESSED
			&& m_mouseButtonInfo.at(button).m_stateChangeFrame == LGL_SERVICE(Timer).getFrameCount();
	}

	bool InputManager::isMouseButtonReleased(const EMouseButton button) const
	{
		return m_mouseButtonInfo.contains(button)
			&& m_mouseButtonInfo.at(button).m_buttonState == EMouseButtonState::MOUSE_RELEASED
			&& m_mouseButtonInfo.at(button).m_stateChangeFrame == LGL_SERVICE(Timer).getFrameCount();
	}

	void InputManager::clearStates()
	{
		m_keyInfos.clear();
		m_scanCodeInfo.clear();
		m_mouseButtonInfo.clear();
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
		const size_t currentFrame = LGL_SERVICE(Timer).getFrameCount();

		m_keyInfos[key] = { state, currentFrame };
		m_scanCodeInfo[scanCode] = { state, currentFrame };
	}

	void InputManager::mouseButtonCallback(const EMouseButton button, const EMouseButtonState state, EInputModifier)
	{
		m_mouseButtonInfo[button] = { state, LGL_SERVICE(Timer).getFrameCount() };
	}
}

#pragma once
#include <unordered_map>

#include "EInputModifier.h"
#include "EKey.h"
#include "EKeyState.h"
#include "EMouseButton.h"
#include "EMouseButtonState.h"
#include "Vector/Vector2.h"

namespace LibGL::Application
{
	class Window;

	class InputManager
	{
	public:
		/**
		 * \brief Creates an input manager instance linked to the given window
		 * \param window A reference to the window linked to this input manager
		 */
		explicit InputManager(Window& window);

		/**
		 * \brief Destroys the input manager
		 */
		~InputManager();

		/**
		 * \brief Updates cached mouse data
		 */
		void updateMouse();

		/**
		 * \brief Resets the input manager to consider the mouse as not moved
		 */
		void resetFirstMouse();

		/**
		 * \brief Gets the cached mouse position
		 * \return The cached mouse position
		 */
		LibMath::Vector2 getMousePosition() const;

		/**
		 * \brief Gets the delta between the current and last mouse positions
		 * \return The difference between the current and previous mouse positions
		 */
		LibMath::Vector2 getMouseDelta() const;

		/**
		 * \brief Gets the layout dependent scan code associated with the given key
		 * \param key The key we want the layout dependent scan code of
		 * \return The layout dependent scan code of the given key
		 */
		static int getScanCode(EKey key);

		/**
		 * \brief Checks whether a given key is down
		 * \param key The key to check
		 * \return True if the key is down. False otherwise.
		 */
		bool isKeyDown(EKey key) const;

		/**
		 * \brief Checks whether a given key is up
		 * \param key The key to check
		 * \return True if the key is up. False otherwise
		 */
		bool isKeyUp(EKey key) const;

		/**
		 * \brief Checks whether the key with a given scan code is down
		 * \param scanCode The scan code of the key to check
		 * \return True if the key is down. False otherwise.
		 */
		bool isKeyDown(int scanCode) const;

		/**
		 * \brief Checks whether the key with a given scan code is up
		 * \param scanCode The scan code of the key to check
		 * \return True if the key is up. False otherwise.
		 */
		bool isKeyUp(int scanCode) const;

		/**
		 * \brief Checks whether a given mouse button is pressed
		 * \param button The mouse button to check
		 * \return True if the mouse button is pressed. False otherwise.
		 */
		bool isMouseButtonPressed(EMouseButton button) const;

		/**
		 * \brief Checks whether a given mouse button is released
		 * \param button The mouse button to check
		 * \return True if the mouse button is released. False otherwise.
		 */
		bool isMouseButtonReleased(EMouseButton button) const;

		/**
		 * \brief Clears the cached input states
		 * \note Call this after each frame to make pressed and released functions
		 * return true only if the action was performed in the same frame.
		 */
		void clearStates();

	private:
		Window&				m_window;

		LibMath::Vector2	m_mousePos;
		LibMath::Vector2	m_lastMousePos;
		LibMath::Vector2	m_mouseDelta;
		bool				m_isFirstMouse;

		uint64_t			m_keyCallbackId = 0;
		uint64_t			m_mouseButtonCallbackId = 0;

		std::unordered_map<EKey, EKeyState>					m_keyStates;
		std::unordered_map<int, EKeyState>					m_scanCodeStates;
		std::unordered_map<EMouseButton, EMouseButtonState>	m_mouseButtonStates;

		/**
		 * \brief Window onKey callback - caches the given key's state
		 * \param key The key that was interacted with
		 * \param scanCode The key's scan code
		 * \param state The new state of the key
		 * \param mods The input modifiers of the key
		 */
		void keyCallback(EKey key, int scanCode, EKeyState state, EInputModifier mods);

		/**
		 * \brief Window onMouseButton callback - caches the given mouse button's state
		 * \param button The mouse button that was interacted with
		 * \param state The new state of the mouse button
		 * \param mods The input modifiers of the mouse button
		 */
		void mouseButtonCallback(EMouseButton button, EMouseButtonState state, EInputModifier mods);
	};
}

#pragma once
#include <stdexcept>
#include <glad/glad.h>

#include "Window.h"
#include "Utility/Timer.h"

namespace LibGL::Application
{
	namespace Exceptions
	{
		class GLADInitFailed : public std::runtime_error
		{
		public:
			GLADInitFailed()
				: runtime_error("Glad initialization failed") {}

			explicit GLADInitFailed(const std::string& message)
				: runtime_error(message.c_str()) {}

			explicit GLADInitFailed(const char* message)
				: runtime_error(message) {}
		};
	}

	class IContext
	{
	public:
		std::unique_ptr<Window>			m_window;
		std::unique_ptr<Utility::Timer>	m_timer;

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

	private:

		/**
		 * \brief OpenGL debug output callback
		 * \param source The debug message's source
		 * \param type The debug message's type
		 * \param id The debug message's id (error/warning code, etc...)
		 * \param severity The debug message's severity
		 * \param length The debug message's length
		 * \param message The debug message
		 * \param userParam Addition user parameters
		 */
		static void APIENTRY glDebugOutput(uint32_t source, uint32_t type, uint32_t id,
		                                   uint32_t severity, int length, const char* message,
		                                   const void* userParam);
	};
}

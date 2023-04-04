#pragma once

#include <filesystem>
#include <fstream>

#define DEBUG_LOG(format, ...) My::Debug::Log::debugLog(__FILE__, __LINE__, format, ##__VA_ARGS__)

namespace My::Debug
{
	class Log
	{
	public:
		~Log();

		/**
		 * \brief Sets the given file as the log output.
		 * \param filePath The log file's path
		 */
		static void			openFile(std::filesystem::path const& filePath);

		/**
		 * \brief Closes the current log output file.
		 */
		static void			closeFile();

		/**
		 * \brief Logs a message with the given format following printf's syntax.
		 * \param format The format of the message
		 * \param args Additional arguments to insert into the message
		 */
		template <typename ... Args>
		static void			print(const char* format, Args ... args);

		/**
		 * \brief Logs a message with the given format following printf's syntax.
		 * Appends the given file path and line at the beginning of the message
		 * \param file The file for which the function was called
		 * \param line The line for which the function was called
		 * \param format The format of the message
		 * \param args Additional arguments to insert into the message
		 */
		template			<typename ... Args>
		static void			debugLog(const char* file, size_t line, const char* format, Args ... args);

	private:
		Log() = default;

		/**
		 * \brief Accessor to a Logger singleton
		 * \return A reference to the current Logger instance
		 */
		static Log&			getInstance();

		/**
		 * \brief Deletes the Logger singleton
		 */
		static void			removeInstance();

		std::ofstream		m_file;
		inline static Log*	m_instance = nullptr;
	};
}

#include "Debug/Log.inl"

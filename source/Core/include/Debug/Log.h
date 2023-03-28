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

		template <typename ... Args>
		static void			print(const char* format, Args ... args);

		template			<typename ... Args>
		static void			debugLog(const char* file, size_t line, const char* format, Args ... args);

	private:
		Log() = default;

		static Log&			getInstance();
		static void			removeInstance();

		std::ofstream		m_file;
		inline static Log*	m_instance = nullptr;
	};
}

#include "Debug/Log.inl"

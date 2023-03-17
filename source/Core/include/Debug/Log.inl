#pragma once
#include "Debug/Log.h"
#include <iostream>

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace My::Debug
{
	template<typename ... Args>
	std::string Log::Format(const char* format, Args ... args)
	{
		// If no template parameters are passed
		// return the format string as is to avoid unnecessary allocation
		if constexpr (sizeof...(Args) == 0)
		{
			return std::string(format);
		}
		else
		{
			// get the formatted text's size
			const int intSize = std::snprintf(nullptr, 0, format, args...) + 1;

			if (intSize <= 0)
				throw std::runtime_error("Unable to print to log - formatting failed.");

			// Create a buffer of the computed size
			const size_t bufferSize = static_cast<size_t>(intSize);
			char* const buffer = new char[bufferSize];

			// Write the formatted string in the buffer
			std::snprintf(buffer, bufferSize, format, args...);

			// Copy the buffer data into an std::string
			std::string message(buffer, buffer + bufferSize - 1);

			// Free the buffer
			delete[] buffer;

			return message;
		}
	}

	template <typename ... Args>
	void Log::Print(const char* format, Args ... args)
	{
		const std::string message = Log::Format(format, args...);

		std::cout << message << std::flush;

		if (GetInstance().m_file.is_open())
			GetInstance().m_file << message << std::flush;
	}

	template <typename ... Args>
	void Log::DebugLog(const char* file, const size_t line, const char* format, Args... args)
	{
		std::string message = Log::Format(format, args...);
		message = Log::Format("%s(%d): %s", file, line, message.c_str());

		Log::Print(message.c_str());

#ifdef _WINDOWS_
		OutputDebugString(message.c_str());
#endif
	}
}
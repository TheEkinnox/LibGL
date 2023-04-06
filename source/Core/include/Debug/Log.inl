#pragma once
#include "Debug/Log.h"
#include "Utility/utility.h"
#include <iostream>

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN	// Disables unnecessary windows features
#define NOMINMAX			// Avoids conflicts with LibMath's min and max
#undef APIENTRY				// Avoids conflicts with some other libs (e.g: GLAD)
#include <Windows.h>
#endif

namespace LibGL::Debug
{
	template <typename ... Args>
	void Log::print(const char* format, Args ... args)
	{
		const std::string message = Utility::formatString(format, args...);

		std::cout << message << std::flush;

		if (getInstance().m_file.is_open())
			getInstance().m_file << message << std::flush;
	}

	template <typename ... Args>
	void Log::debugLog(const char* file, const size_t line, const char* format, Args... args)
	{
		std::string message = Utility::formatString(format, args...);
		message = Utility::formatString("%s(%d): %s", file, line, message.c_str());

		Log::print(message.c_str());

#ifdef _WINDOWS_
		OutputDebugString(message.c_str());
#endif
	}
}
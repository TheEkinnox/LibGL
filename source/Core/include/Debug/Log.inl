#pragma once
#include "Debug/Log.h"
#include "Utility/utility.h"

#include <iostream>

namespace LibGL::Debug
{
    template <typename... Args>
    void Log::print(const char* format, Args... args)
    {
        const std::string message = Utility::formatString(format, args...);

        std::cout << message << std::flush;

        if (getInstance().m_file.is_open())
            getInstance().m_file << message << std::flush;
    }

    template <typename... Args>
    void Log::debugLog(const char* file, const size_t line, const char* format, Args... args)
    {
        std::string message = Utility::formatString(format, args...);
        message             = Utility::formatString("%s(%d): %s", file, line, message.c_str());

        print(message.c_str());
    }
}

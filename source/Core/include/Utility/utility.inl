#pragma once
#include "Utility/utility.h"

#include <algorithm>
#include <stdexcept>

namespace LibGL::Utility
{
    template <typename... Args>
    std::string formatString(const char* const format, Args... args)
    {
        // If no template parameters are passed
        // return the format string as is to avoid unnecessary allocation
        if constexpr (sizeof...(Args) == 0)
        {
            return format;
        }
        else
        {
            // get the formatted text's size
            const int intSize = std::snprintf(nullptr, 0, format, args...) + 1;

            if (intSize <= 0)
                throw std::runtime_error("Unable to format string.");

            // Create a buffer of the computed size
            const size_t bufferSize = intSize;
            const auto   buffer     = static_cast<char*>(alloca(bufferSize * sizeof(char)));

            // Write the formatted string in the buffer
            std::snprintf(buffer, bufferSize, format, args...);

            // Copy the buffer data into an std::string
            std::string message(buffer, buffer + bufferSize - 1);

            return message;
        }
    }

    template <class CompareFunc>
    void trimStringStart(std::string& str, CompareFunc compareFunc)
    {
        str.erase(str.begin(), std::ranges::find_if_not(str, compareFunc));
    }

    template <class CompareFunc>
    void trimStringEnd(std::string& str, CompareFunc compareFunc)
    {
        str.erase(std::find_if_not(str.rbegin(), str.rend(), compareFunc).base(), str.end());
    }

    template <class CompareFunc>
    void trimString(std::string& str, CompareFunc compareFunc)
    {
        trimStringEnd(str, compareFunc);
        trimStringStart(str, compareFunc);
    }
}

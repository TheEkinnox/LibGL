#pragma once

#include "Utility/utility.h"

namespace My::Utility
{
	template<typename ... Args>
	std::string formatString(const char* const format, Args ... args)
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
}

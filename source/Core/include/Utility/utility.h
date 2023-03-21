#pragma once

#include <vector>
#include <string>

namespace My::Utility
{
	/**
	 * \brief Splits the given string using the given delimiter
	 * \param str The string to split
	 * \param delimiter The delimiter to use to split the string
	 * \param includeEmpty Whether empty substrings should be included in the result
	 * \return A vector containing the sub-strings
	 */
	std::vector<std::string> splitString(const std::string& str, char const* delimiter, bool includeEmpty);
}

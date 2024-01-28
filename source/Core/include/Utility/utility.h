#pragma once
#include <functional>
#include <vector>
#include <string>

namespace LibGL::Utility
{
    /**
     * \brief Formats a string using the printf syntax
     * \tparam Args The arguments to insert into the format string
     * \param format The format string (follows printf syntax)
     * \param args The arguments to insert into the format string
     * \return The formatted string
     */
    template <class... Args>
    std::string formatString(const char* format, Args... args);

    /**
     * \brief Splits the given string using the given delimiter
     * \param str The string to split
     * \param delimiter The delimiter to use to split the string
     * \param includeEmpty Whether empty substrings should be included in the result
     * \return A vector containing the sub-strings
     */
    std::vector<std::string> splitString(const std::string& str, const std::string& delimiter, bool includeEmpty);

    /**
     * \brief Trims the beginning of the given string using the given compare func
     * \param str The string to trim
     * \param compareFunc The function used to check if a character should be trimmed
     */
    inline void trimStringStart(std::string& str, const std::function<bool(char)>& compareFunc = &isspace);

    /**
     * \brief Trims the end of the given string using the given compare func
     * \param str The string to trim
     * \param compareFunc The function used to check if a character should be trimmed
     */
    inline void trimStringEnd(std::string& str, const std::function<bool(char)>& compareFunc = &isspace);

    /**
     * \brief Trims the given string using the given compare func
     * \param str The string to trim
     * \param compareFunc The function used to check if a character should be trimmed
     */
    inline void trimString(std::string& str, const std::function<bool(char)>& compareFunc = &isspace);

    /**
     * \brief Reads the lines of the given text file
     * \param fileName The file's path
     * \return A vector containing the file's lines
     */
    std::vector<std::string> readFile(const std::string& fileName);
}

#include "Utility/utility.inl"

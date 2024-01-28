#include "Utility/utility.h"

#include <fstream>
#include <iostream>

namespace LibGL::Utility
{
    std::vector<std::string> splitString(const std::string& str, const std::string& delimiter, const bool includeEmpty)
    {
        std::vector<std::string> result{};

        if (str.empty())
        {
            if (includeEmpty)
                result.push_back(str);

            return result;
        }

        size_t start = 0;
        size_t end = str.find(delimiter, start);

        while ((start) < str.size())
        {
            std::string token = str.substr(start, end - start);

            if (includeEmpty || !token.empty())
                result.emplace_back(std::move(token));

            if (end == std::string::npos)
                break;

            start = end + delimiter.size();
            end = str.find(delimiter, start);
        }

        return result;
    }

    std::vector<std::string> readFile(const std::string& fileName)
    {
        std::ifstream fs(fileName);

        if (!fs.is_open())
        {
            std::cerr << "Unable to open file at path \"" << fileName << '\"' << std::endl;
            return {};
        }

        std::vector<std::string> lines;
        std::string              line;

        while (std::getline(fs, line))
            lines.emplace_back(std::move(line));

        return lines;
    }
}

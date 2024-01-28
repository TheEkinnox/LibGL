#include "Debug/Log.h"

namespace LibGL::Debug
{
    Log::~Log()
    {
        if (m_file.is_open())
            m_file.close();
    }

    void Log::openFile(const std::filesystem::path& filePath)
    {
        closeFile();

        getInstance().m_file.open(filePath);
    }

    void Log::closeFile()
    {
        if (getInstance().m_file.is_open())
            getInstance().m_file.close();
    }

    Log& Log::getInstance()
    {
        static Log instance;
        return instance;
    }
}

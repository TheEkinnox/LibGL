#include "Debug/Log.h"
#include <cstdlib> // atexit

#include "Debug/Assertion.h"

namespace LibGL::Debug
{
	Log::~Log()
	{
		if (m_file.is_open())
			m_file.close();
	}

	void Log::openFile(std::filesystem::path const& filePath)
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
		if (s_instance == nullptr)
		{
			s_instance = new Log();
			ASSERT(atexit(removeInstance) == 0);
		}

		return *s_instance;
	}

	void Log::removeInstance()
	{
		delete s_instance;
		s_instance = nullptr;
	}
}

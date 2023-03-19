#include "Debug/Log.h"
#include <cstdlib> // atexit

#include "Debug/Assertion.h"

namespace My::Debug
{
	Log::~Log()
	{
		if (m_file.is_open())
			m_file.close();
	}

	void Log::openFile(std::filesystem::path const& filePath)
	{
		if (getInstance().m_file.is_open())
			getInstance().m_file.close();

		getInstance().m_file.open(filePath);
	}

	Log& Log::getInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new Log();
			ASSERT(atexit(removeInstance) == 0);
		}

		return *m_instance;
	}

	void Log::removeInstance()
	{
		delete m_instance;
	}
}

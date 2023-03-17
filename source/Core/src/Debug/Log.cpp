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

	void Log::OpenFile(std::filesystem::path const& filePath)
	{
		if (GetInstance().m_file.is_open())
			GetInstance().m_file.close();

		GetInstance().m_file.open(filePath);
	}

	Log& Log::GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new Log();
			ASSERT(atexit(RemoveInstance) == 0);
		}

		return *m_instance;
	}

	void Log::RemoveInstance()
	{
		delete m_instance;
	}
}

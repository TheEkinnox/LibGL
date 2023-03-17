#include "Debug/Log.h"

namespace My::Debug
{
	Log::~Log()
	{
		if (m_file.is_open())
			m_file.close();
	}

	void Log::OpenFile(std::filesystem::path const& filePath)
	{
		if (m_file.is_open())
			m_file.close();

		m_file.open(filePath);
	}
}

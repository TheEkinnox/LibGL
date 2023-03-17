#include "ResourcesManager.h"

namespace My::Resources
{
	void ResourceManager::Delete(const std::string& fileName)
	{
		if (m_resources.find(fileName) != m_resources.end())
		{
			m_resources[fileName].reset();
			m_resources.erase(fileName);
		}
	}
}

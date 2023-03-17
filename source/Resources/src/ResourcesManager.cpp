#include "ResourcesManager.h"

namespace My::Resources
{
	ResourceManager::ResourceManager(const ResourceManager& other)
		: m_resources(other.m_resources)
	{
	}

	ResourceManager::ResourceManager(ResourceManager&& other) noexcept
		: m_resources(std::move(other.m_resources))
	{
	}

	ResourceManager::~ResourceManager()
	{
		for (const auto& [_, resource] : m_resources)
			delete resource;

		m_resources.clear();
	}

	ResourceManager& ResourceManager::operator=(const ResourceManager& other)
	{
		if (&other == this)
			return *this;

		m_resources = other.m_resources;

		return *this;
	}

	ResourceManager& ResourceManager::operator=(ResourceManager&& other) noexcept
	{
		if (&other == this)
			return *this;

		m_resources = std::move(other.m_resources);

		return *this;
	}

	void ResourceManager::Delete(const std::string& fileName)
	{
		if (m_resources.find(fileName) != m_resources.end())
		{
			delete m_resources[fileName];
			m_resources.erase(fileName);
		}
	}
}

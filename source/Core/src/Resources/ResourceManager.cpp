#include "Resources/ResourceManager.h"

#include <ranges>

namespace LibGL::Resources
{
    ResourceManager::ResourceManager(ResourceManager&& other) noexcept
        : m_resources(std::move(other.m_resources))
    {
    }

    ResourceManager::~ResourceManager()
    {
        for (const auto& resource : m_resources | std::views::values)
            delete resource;

        m_resources.clear();
    }

    ResourceManager& ResourceManager::operator=(ResourceManager&& other) noexcept
    {
        if (&other == this)
            return *this;

        m_resources = std::move(other.m_resources);

        return *this;
    }

    void ResourceManager::remove(const std::string& fileName)
    {
        if (m_resources.contains(fileName))
        {
            delete m_resources[fileName];
            m_resources.erase(fileName);
        }
    }
}

#pragma once
#include "Resources/ResourceManager.h"
#include "Resources/IResource.h"

#include "Utility/ServiceLocator.h"
#include "Utility/ThreadPool.h"

namespace LibGL::Resources
{
    template <typename T>
    T* ResourceManager::load(const std::string& fileName, const bool initOnLoad)
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

        {
            std::lock_guard lock(m_resourcesMutex);

            if (m_resources.contains(fileName))
            {
                delete m_resources[fileName];
                m_resources[fileName] = nullptr;
            }
        }

        T* ptr = new T();

        if (!ptr->load(fileName) || (initOnLoad && !ptr->init()))
        {
            delete ptr;
            return nullptr;
        }

        {
            std::lock_guard lock(m_resourcesMutex);
            m_resources[fileName] = ptr;
        }

        return ptr;
    }

    template <typename T, typename ReturnT>
    std::future<ReturnT*> ResourceManager::loadInBackground(const std::string& fileName)
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);
        static_assert(std::is_same_v<T, ReturnT> || std::is_base_of_v<ReturnT, T>);

        Utility::ThreadPool& threadPool = LGL_SERVICE(Utility::ThreadPool);
        return threadPool.enqueue([this, fileName]
        {
            return static_cast<ReturnT*>(load<T>(fileName, false));
        });
    }

    template <typename T>
    T* ResourceManager::get(const std::string& fileName) const
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

        std::lock_guard lock(m_resourcesMutex);

        const auto it = m_resources.find(fileName);
        if (it == m_resources.end())
            return nullptr;

        return reinterpret_cast<T*>(it->second);
    }

    template <typename T>
    T* ResourceManager::getOrCreate(const std::string& fileName)
    {
        T* resource = get<T>(fileName);

        if (resource != nullptr)
            return resource;

        return load<T>(fileName);
    }
}

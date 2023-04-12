#pragma once
#include "ResourcesManager.h"
#include "IResource.h"

namespace LibGL::Resources
{
	template <typename T>
	T* ResourceManager::create(const std::string& fileName)
	{
		static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

		if (m_resources.contains(fileName))
			delete m_resources[fileName];

		T* ptr = new T();

		if (!ptr->loadFromFile("assets/" + fileName))
		{
			delete ptr;
			return nullptr;
		}

		m_resources[fileName] = ptr;

		return ptr;
	}

	template <typename T>
	T* ResourceManager::get(const std::string& fileName) const
	{
		static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

		if (!m_resources.contains(fileName))
			return nullptr;

		return reinterpret_cast<T*>(m_resources.at(fileName));
	}

	template <typename T>
	T* ResourceManager::getOrCreate(const std::string& fileName)
	{
		T* resource = get<T>(fileName);

		if (resource != nullptr)
			return resource;

		return create<T>(fileName);
	}
}

#pragma once
#include "ResourcesManager.h"
#include "IResource.h"
#include "Debug/Assertion.h"

namespace My::Resources
{
	template <typename T>
	T* ResourceManager::Create(const std::string& fileName)
	{
		static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

		if (m_resources.find(fileName) != m_resources.end())
			delete m_resources[fileName];

		T* ptr = new T();

		ptr->LoadFromFile(fileName);

		m_resources[fileName] = ptr;

		return ptr;
	}

	template <typename T>
	T* ResourceManager::Get(const std::string& fileName) const
	{
		static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

		if (m_resources.find(fileName) == m_resources.end())
			return nullptr;

		return m_resources[fileName];
	}
}

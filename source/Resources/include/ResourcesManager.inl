#pragma once
#include "ResourcesManager.h"
#include "IResource.h"
#include "Debug/Assertion.h"

namespace My::Resources
{
	template <typename T>
	T* ResourceManager::Create(const std::string& fileName)
	{
		ASSERT((std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>));

		if (m_resources.find(fileName) != m_resources.end())
			m_resources[fileName].reset();

		ResourcePtr ptr = std::make_unique<T>();

		ptr->LoadFromFile(fileName);

		m_resources[fileName] = ptr;

		return ptr.get();
	}

	template <typename T>
	T* ResourceManager::Get(const std::string& fileName) const
	{
		ASSERT((std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>));

		if (m_resources.find(fileName) == m_resources.end())
			return nullptr;

		return m_resources[fileName].get();
	}
}

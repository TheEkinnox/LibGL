#pragma once
#include <string>
#include <unordered_map>

namespace My::Resources
{
	class IResource;

	class ResourceManager
	{
	public:
		ResourceManager() = default;
		ResourceManager(const ResourceManager& other);
		ResourceManager(ResourceManager&& other) noexcept;
		~ResourceManager();

		ResourceManager& operator=(const ResourceManager& other);
		ResourceManager& operator=(ResourceManager&& other) noexcept;

		template	<typename T>
		T*			create(const std::string& fileName);

		template	<typename T>
		T*			get(const std::string& fileName) const;

		void		remove(const std::string& fileName);

	private:
		using		ResourcePtr = IResource*;
		using		ResourceMap = std::unordered_map<std::string, ResourcePtr>;

		ResourceMap	m_resources;
	};
}

#include "ResourcesManager.inl"

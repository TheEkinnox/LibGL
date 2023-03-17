#pragma once
#include <string>
#include <memory> // unique_ptr
#include <unordered_map>

namespace My::Resources
{
	class IResource;

	class ResourceManager
	{
	public:
		template	<typename T>
		T*			Create(const std::string& fileName);

		template	<typename T>
		T*			Get(const std::string& fileName) const;

		void		Delete(const std::string& fileName);

	private:
		using		ResourcePtr = std::unique_ptr<IResource>;
		using		ResourceMap = std::unordered_map<std::string, ResourcePtr>;

		ResourceMap	m_resources;
	};
}

#include "ResourcesManager.inl"

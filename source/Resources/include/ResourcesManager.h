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

		/**
		 * \brief Tries to create the resource with the given file name.
		 * \param fileName The name of the resource's file
		 * \return A pointer to the resource on success, nullptr otherwise.
		 */
		template <typename T>
		T* create(const std::string& fileName);


		/**
		 * \brief Tries to find the resource with the given file name.
		 * \param fileName The name of the resource's file
		 * \return A pointer to the resource on success, nullptr otherwise.
		 */
		template <typename T>
		T* get(const std::string& fileName) const;

		/**
		 * \brief Tries to find the resource with the given file name.
		 * If it can't be found, tries to create it.
		 * \param fileName The name of the resource's file
		 * \return A pointer to the resource on success, nullptr otherwise.
		 */
		template <typename T>
		T* getOrCreate(const std::string& fileName);

		/**
		 * \brief Removes the resource with the given file name from the manager
		 * \param fileName The name of the resource's file
		 */
		void remove(const std::string& fileName);

	private:
		using		ResourcePtr = IResource*;
		using		ResourceMap = std::unordered_map<std::string, ResourcePtr>;

		ResourceMap	m_resources;
	};
}

#include "ResourcesManager.inl"

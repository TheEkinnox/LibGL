#pragma once
#include <future>
#include <string>
#include <unordered_map>

namespace LibGL::Resources
{
    class IResource;

    class ResourceManager
    {
    public:
        ResourceManager() = default;
        ResourceManager(const ResourceManager& other) = delete;
        ResourceManager(ResourceManager&& other) noexcept;
        ~ResourceManager();

        ResourceManager& operator=(const ResourceManager& other) = delete;
        ResourceManager& operator=(ResourceManager&& other) noexcept;

        /**
         * \brief Tries to create the resource with the given file name.
         * \param fileName The name of the resource's file
         * \param initOnLoad Whether or not the resource should be initialized on load
         * \return A pointer to the resource on success, nullptr otherwise.
         */
        template <typename T>
        T* load(const std::string& fileName, bool initOnLoad = true);

        /**
         * \brief Tries to load the resource with the given file name using multithreading.
         * IMPORTANT: The loaded resource is NOT initialized
         * \param fileName The name of the resource's file
         * \return A future returning a pointer to the resource on success or nullptr otherwise.
         */
        template <typename T, typename ReturnT = T>
        std::future<ReturnT*> loadInBackground(const std::string& fileName);

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
        using ResourcePtr = IResource*;
        using ResourceMap = std::unordered_map<std::string, ResourcePtr>;

        mutable std::mutex m_resourcesMutex;
        ResourceMap        m_resources;
    };
}

#include "Resources/ResourceManager.inl"

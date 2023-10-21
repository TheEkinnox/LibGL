#pragma once

#include <unordered_map>
#include <string>

#define REGISTER_RESOURCE_TYPE(Type) static uint8_t reg_##Type = (LibGL::Resources::IResource::registerType<Type>(), 0)

namespace LibGL::Resources
{
    class IResource
    {
    public:
        /**
         * \brief Destroys the resource
         */
        virtual ~IResource() = default;

        /**
         * \brief Loads the resource from the given file
         * \param fileName The resource file's path
         * \return True if the resource was successfully loaded. False otherwise.
         */
        virtual bool load(const char* fileName) = 0;

        /**
         * \brief Loads the resource from the given file
         * \param fileName The resource file's path
         * \return True on success. False otherwise.
         */
        bool load(const std::string& fileName);

        /**
         * \brief Initializes the resource
         * \return True on success. False otherwise.
         */
        virtual bool init()
        {
            return true;
        }

        /**
         * \brief Registers the given resource type (required for the create function)
         * \tparam T The Resource type to register
         */
        template <typename T>
        static constexpr void registerType();

        /**
         * \brief Tries to allocate a resource of the given registered resource type.
         * \param type The type of the resource to create
         * \return A pointer to the allocated resource on success, nullptr otherwise
         */
        inline static IResource* create(const std::string& type);

    private:
        using AllocFunc = IResource* (*)();
        using TypeMap = std::unordered_map<std::string, AllocFunc>;

        inline static TypeMap m_resourceTypes{};
    };
}

#include "IResource.inl"

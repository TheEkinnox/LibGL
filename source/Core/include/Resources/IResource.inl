#pragma once
#include <type_traits>

#include <Debug/Assertion.h>

#include "Resources/IResource.h"

namespace LibGL::Resources
{
    inline bool IResource::load(const std::string& fileName)
    {
        return load(fileName.c_str());
    }

    template <typename T>
    constexpr void IResource::registerType()
    {
        static_assert(std::is_base_of_v<IResource, T>);

        const std::string name = typeid(T).raw_name();

        ASSERT(!m_resourceTypes.contains(name), "Resource type \"%s\" has already been registered", name.c_str());

        m_resourceTypes[name] = []() -> IResource* {
            return new T();
        };
    }

    inline IResource* IResource::create(const std::string& type)
    {
        return m_resourceTypes.contains(type) ? m_resourceTypes[type]() : nullptr;
    }
}

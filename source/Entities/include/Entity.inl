#pragma once

#include "Component.h"
#include "Entity.h"

namespace LibGL
{
	inline Entity::Entity(Entity* parent, const Transform& transform) :
		SceneNode(parent, transform)
	{
	}

	template <typename T, typename ... Args>
	T& Entity::addComponent(Args&&... args)
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		m_components.push_back(std::make_shared<T>(args));

		return *m_components.back();
	}

	template <typename T>
	void Entity::removeComponent()
	{
		const T* firstComponent = getComponent<T>();

		if (nullptr != firstComponent)
			removeComponent(*firstComponent);
	}

	inline void Entity::removeComponent(const Component& component)
	{
		const auto findIfFunc = [component](const ComponentPtr& ptr)
		{
			return ptr.get() == &component;
		};

		const auto found = std::ranges::find_if(m_components, findIfFunc);

		if (found != m_components.end())
			m_components.erase(found);
	}

	inline void Entity::update()
	{
		for (const auto& component : m_components)
			component->update();

		SceneNode::update();
	}


	template <typename T>
	T* Entity::getComponent()
	{
		for (const auto& component : m_components)
		{
			if (typeid(component.get()) == typeid(T*))
				return static_cast<T*>(component.get());
		}

		return nullptr;
	}

	template <typename T>
	std::vector<std::shared_ptr<T>> Entity::getComponents()
	{
		std::vector<std::shared_ptr<T>> components;

		for (const auto& component : m_components)
		{
			if (typeid(component.get()) == typeid(T*))
				components.push_back(std::dynamic_pointer_cast<T>(component));
		}

		return components;
	}
}

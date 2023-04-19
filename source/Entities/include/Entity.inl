#pragma once

#include "Component.h"
#include "Entity.h"

namespace LibGL
{
	inline Entity::Entity(Entity* parent, const Transform& transform) :
		Node(parent), Transform(transform)
	{
	}

	inline Entity::~Entity()
	{
		if (!m_components.empty())
			m_components.clear();
	}

	inline LibMath::Transform Entity::getGlobalTransform() const
	{
		return m_globalTransform;
	}

	template <typename T, typename ... Args>
	T& Entity::addComponent(Args&&... args)
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		m_components.push_back(std::make_shared<T>(*this, args...));

		return static_cast<T&>(*m_components.back());
	}

	template <typename T>
	void Entity::removeComponent()
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		const T* firstComponent = getComponent<T>();

		if (nullptr != firstComponent)
			removeComponent(*firstComponent);
	}

	inline void Entity::removeComponent(const Component& component)
	{
		if (m_components.empty())
			return;

		const auto findFunc = [component](const ComponentPtr& ptr)
		{
			return *ptr == component;
		};

		std::erase_if(m_components, findFunc);
	}

	inline void Entity::removeComponent(const Component::ComponentId id)
	{
		if (m_components.empty())
			return;

		const auto findFunc = [id](const ComponentPtr& ptr)
		{
			return ptr->getId() == id;
		};

		std::erase_if(m_components, findFunc);
	}

	inline void Entity::update()
	{
		for (const auto& component : m_components)
			component->update();

		for (Node* child : getChildren())
			reinterpret_cast<Entity*>(child)->update();
	}

	inline void Entity::onChange()
	{
		Transform::onChange();

		updateGlobalTransform();

		for (auto* child : getChildren())
			if (child != nullptr)
				reinterpret_cast<Entity*>(child)->updateGlobalTransform();
	}

	inline void Entity::updateGlobalTransform()
	{
		m_globalTransform = static_cast<Transform>(*this);

		const Entity* castParent = reinterpret_cast<Entity*>(getParent());

		if (castParent != nullptr)
		{
			const Transform parentTransform = castParent->getGlobalTransform();
			m_globalTransform.translate(parentTransform.getPosition());
			m_globalTransform.rotate(parentTransform.getRotation());
			m_globalTransform.scale(parentTransform.getScale());
		}
	}

	template <typename T>
	T* Entity::getComponent()
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		for (const auto& component : m_components)
		{
			if (typeid(component.get()) == typeid(T*) || dynamic_cast<T*>(component.get()) != nullptr)
				return dynamic_cast<T*>(component.get());
		}

		return nullptr;
	}

	template <typename T>
	T* Entity::getComponent(const Component::ComponentId id)
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		for (const auto& component : m_components)
		{
			if (id == component->getId() &&
				(typeid(component.get()) == typeid(T*) ||
					dynamic_cast<T*>(component.get()) != nullptr))
			{
				return dynamic_cast<T*>(component.get());
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<std::shared_ptr<T>> Entity::getComponents()
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		std::vector<std::shared_ptr<T>> components;

		for (const auto& component : m_components)
		{
			if (typeid(component.get()) == typeid(T*) || dynamic_cast<T*>(component.get()) != nullptr)
				components.push_back(std::dynamic_pointer_cast<T>(component));
		}

		return components;
	}

	inline void Entity::addChild(Node& child)
	{
		Node::addChild(child);
		reinterpret_cast<Entity&>(child).updateGlobalTransform();
	}

	inline void Entity::removeChild(Node& child)
	{
		Node::removeChild(child);
		reinterpret_cast<Entity&>(child).updateGlobalTransform();
	}
}

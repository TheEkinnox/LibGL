#include "Entity.h"

namespace LibGL
{
	Entity::Entity(Entity* parent, const Transform& transform) :
		Node(parent), Transform(transform)
	{
	}

	Entity::Entity(const Entity& other) :
		Node(other), Transform(other),
		m_components(other.m_components)
	{
		if (!m_components.empty())
			for (const auto& component : m_components)
				component->m_owner = *this;

		updateGlobalTransform();
	}

	Entity::Entity(Entity&& other) noexcept :
		Node(std::move(other)), Transform(std::move(other)),
		m_components(std::move(other.m_components))
	{
		if (!m_components.empty())
			for (const auto& component : m_components)
				component->m_owner = *this;

		updateGlobalTransform();
	}

	Entity::~Entity()
	{
		if (!m_components.empty())
			m_components.clear();
	}

	Entity& Entity::operator=(const Entity& other)
	{
		if (&other == this)
			return *this;

		Node::operator=(other);
		Transform::operator=(other);

		m_components.clear();

		m_components = other.m_components;

		if (!m_components.empty())
			for (const auto& component : m_components)
				component->m_owner = *this;

		updateGlobalTransform();

		return *this;
	}

	Entity& Entity::operator=(Entity&& other) noexcept
	{
		if (&other == this)
			return *this;

		Node::operator=(other);
		Transform::operator=(other);

		m_components.clear();

		m_components = std::move(other.m_components);
		m_globalTransform = std::move(other.m_globalTransform);

		if (!m_components.empty())
			for (const auto& component : m_components)
				component->m_owner = *this;

		updateGlobalTransform();

		return *this;
	}

	LibMath::Transform Entity::getGlobalTransform() const
	{
		return m_globalTransform;
	}


	void Entity::removeComponent(const Component& component)
	{
		if (m_components.empty())
			return;

		const auto findFunc = [component](const ComponentPtr& ptr)
		{
			return *ptr == component;
		};

		std::erase_if(m_components, findFunc);
	}

	void Entity::removeComponent(const Component::ComponentId id)
	{
		if (m_components.empty())
			return;

		const auto findFunc = [id](const ComponentPtr& ptr)
		{
			return ptr->getId() == id;
		};

		std::erase_if(m_components, findFunc);
	}

	void Entity::update()
	{
		for (const auto& component : m_components)
			component->update();

		for (Node* child : getChildren())
			reinterpret_cast<Entity*>(child)->update();
	}

	void Entity::onChange()
	{
		Transform::onChange();

		updateGlobalTransform();

		for (auto* child : getChildren())
			if (child != nullptr)
				reinterpret_cast<Entity*>(child)->updateGlobalTransform();
	}

	void Entity::updateGlobalTransform()
	{
		m_globalTransform = static_cast<Transform>(*this);

		const Entity* castParent = reinterpret_cast<Entity*>(getParent());

		if (castParent != nullptr)
			m_globalTransform = castParent->getGlobalTransform() * m_globalTransform;
	}

	void Entity::addChild(Node& child)
	{
		Node::addChild(child);
		reinterpret_cast<Entity&>(child).updateGlobalTransform();
	}

	void Entity::removeChild(Node& child)
	{
		Node::removeChild(child);
		reinterpret_cast<Entity&>(child).updateGlobalTransform();
	}
}

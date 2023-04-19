#include "Entity.h"

namespace LibGL
{
	Entity::Entity(Entity* parent, const Transform& transform) :
		Node(parent), Transform(transform)
	{
	}

	Entity::~Entity()
	{
		if (!m_components.empty())
			m_components.clear();
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
		{
			const Transform parentTransform = castParent->getGlobalTransform();
			m_globalTransform.translate(parentTransform.getPosition());
			m_globalTransform.rotate(parentTransform.getRotation());
			m_globalTransform.scale(parentTransform.getScale());
		}
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

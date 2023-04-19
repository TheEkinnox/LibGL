#pragma once
#include <memory> // shared_ptr
#include <vector>

#include "Transform.h"
#include "Component.h"
#include "DataStructure/Node.h"

namespace LibGL
{
	class Entity : public DataStructure::Node, public LibMath::Transform
	{
		typedef std::shared_ptr<Component> ComponentPtr;
		typedef std::vector<ComponentPtr> ComponentList;

	public:
		Entity() = default;
		Entity(Entity* parent, const Transform& transform);
		Entity(const Entity& other);
		Entity(Entity&& other) noexcept;
		~Entity() override;

		Entity& operator=(const Entity& other);
		Entity& operator=(Entity&& other) noexcept;

		/**
		 * \brief Gets the entity's global transformation matrix
		 * \return The entity's global transformation matrix
		 */
		Transform getGlobalTransform() const;

		/**
		 * \brief Adds a component of the given type to the entity
		 * \param args The component's constructor's parameters
		 * \return A reference to the added component
		 */
		template <typename T, typename ... Args>
		T& addComponent(Args&&... args);

		template <typename T>
		void removeComponent();

		void removeComponent(const Component& component);

		void removeComponent(Component::ComponentId id);

		template <typename T>
		T* getComponent();

		template <typename T>
		T* getComponent(Component::ComponentId id);

		template <typename T>
		std::vector<std::shared_ptr<T>> getComponents();

		/**
		 * \brief Adds the given node as a child of the current node
		 * \param child A pointer to the child to add to the current node
		 */
		void addChild(Node& child) override;

		/**
		 * \brief Removes the given node from this node's children
		 * \param child A pointer to the child to remove from the node's children
		 */
		void removeChild(Node& child) override;

		/**
		 * \brief Updates the entity (does nothing by default)
		 */
		virtual void update();

	protected:
		/**
		 * \brief Updates the entity's transformation matrices
		 */
		void onChange() override;

	private:
		ComponentList	m_components;
		Transform		m_globalTransform;

		void updateGlobalTransform();
	};
}

#include "Entity.inl"

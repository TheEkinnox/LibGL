#pragma once

#include "Scene.h"
#include <memory> // shared_ptr
#include <vector>

namespace LibGL
{
	class Entity : public Resources::SceneNode
	{
		typedef std::shared_ptr<Component> ComponentPtr;
		typedef std::vector<ComponentPtr> ComponentList;

	public:
		template <typename T, typename ... Args>
		T& addComponent(Args&&... args);

		template <typename T>
		void removeComponent();

		void removeComponent(const Component& component);

		template <typename T>
		T* getComponent();

		template <typename T>
		std::vector<std::shared_ptr<T>> getComponents();

		virtual void update() override;

	private:
		ComponentList	m_components;
	};
}

#include "Entity.inl"

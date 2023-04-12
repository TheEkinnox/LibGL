#pragma once

namespace LibGL
{
	class Entity;

	class Component
	{
	public:
		Component(const Component& other) = default;
		Component(Component&& other) = default;

		Component& operator=(const Component& other) = default;
		Component& operator=(Component&& other) = default;

		virtual ~Component();

		virtual void update() {}

		bool isActive() const;
		void setActive(bool active);

	protected:
		explicit Component(Entity& owner) : m_owner(owner) {}

		Entity& getOwner() const;

	private:
		Entity& m_owner;
		bool	m_isActive = true;

		virtual void onEnable() {}
		virtual void onDisable() {}
	};
}

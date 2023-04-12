#include "Component.h"

#include "Entity.h"

namespace LibGL
{
	Component::~Component()
	{
		m_owner.removeComponent(*this);
	}

	bool Component::isActive() const
	{
		return m_isActive;
	}

	void Component::setActive(const bool active)
	{
		if (active == m_isActive)
			return;

		m_isActive = active;

		m_isActive ? onEnable() : onDisable();
	}

	Entity& Component::getOwner() const
	{
		return m_owner;
	}
}

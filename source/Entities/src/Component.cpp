#include "Component.h"

#include "Entity.h"

namespace LibGL
{
	Component& Component::operator=(const Component& other)
	{
		if (&other == this)
			return *this;

		m_owner = other.m_owner;
		m_isActive = other.m_isActive;

		return *this;
	}

	Component& Component::operator=(Component&& other) noexcept
	{
		if (&other == this)
			return *this;

		m_owner = std::move(other.m_owner);
		m_isActive = other.m_isActive;

		return *this;
	}

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
#pragma once

#include <SFML\Graphics.hpp>
#include <Components\ComponentBase.h>

class ComponentCollidable : public ComponentBase
{
public:
	ComponentCollidable(ComponentType type, SystemType owningSystem)
		: ComponentBase(type, owningSystem),
		m_AABB(sf::Vector2f(0, 0), sf::Vector2f(16, 16))
	{}
	ComponentCollidable(const ComponentCollidable&) = delete;
	ComponentCollidable& operator=(const ComponentCollidable&) = delete;
	ComponentCollidable(ComponentCollidable&&) = delete;
	ComponentCollidable&& operator=(ComponentCollidable&&) = delete;

	sf::FloatRect m_AABB;
};
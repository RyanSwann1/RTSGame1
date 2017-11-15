#pragma once

#include <Components\ComponentBase.h>
#include <SFML\Graphics.hpp>
class ComponentMovable : public ComponentBase
{
public:
	ComponentMovable(ComponentType type, SystemType owningSystem)
		: ComponentBase(type, owningSystem),
		m_velocity(),
		m_speed(7500, 7500),
		m_movementDirection()
	{}

	sf::Vector2f m_velocity;
	sf::Vector2f m_speed;
	Direction m_movementDirection;
};
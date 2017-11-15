#pragma once

#include <Components\ComponentBase.h>
#include <SFML\Graphics.hpp>

class ComponentPosition : public ComponentBase
{
public:
	ComponentPosition(ComponentType type, SystemType owningSystem)
		: ComponentBase(type, owningSystem),
		m_position()
	{}

	sf::Vector2f m_position;
};
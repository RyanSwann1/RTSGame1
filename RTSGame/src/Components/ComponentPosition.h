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
	ComponentPosition(const ComponentPosition&) = delete;
	ComponentPosition& operator=(const ComponentPosition&) = delete;
	ComponentPosition(ComponentPosition&&) = delete;
	ComponentPosition&& operator=(ComponentPosition&&) = delete;

	sf::Vector2f m_position;
};
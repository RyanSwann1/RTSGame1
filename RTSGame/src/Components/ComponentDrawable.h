#pragma once

#include <SFML\Graphics.hpp>
#include <Components\ComponentBase.h>

class ComponentDrawable : public ComponentBase
{
public:
	ComponentDrawable(ComponentType type, SystemType owningSystem)
		: ComponentBase(type, owningSystem)
	{
		m_rect.setFillColor(sf::Color::Yellow);
	}
	ComponentDrawable(const ComponentDrawable&) = delete;
	ComponentDrawable& operator=(const ComponentDrawable&) = delete;
	ComponentDrawable(ComponentDrawable&&) = delete;
	ComponentDrawable&& operator=(ComponentDrawable&&) = delete;

	sf::RectangleShape m_rect;
};
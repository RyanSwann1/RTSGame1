#pragma once

#include <Components\ComponentBase.h>
class ComponentSelectable : public ComponentBase
{
public:
	ComponentSelectable(ComponentType type, SystemType owningSystem)
		: ComponentBase(type, owningSystem),
		m_selected(false),
		m_selectedRect()
	{
		m_selectedRect.setFillColor(sf::Color::Transparent);
		m_selectedRect.setOutlineColor(sf::Color::Green);
		m_selectedRect.setOutlineThickness(1.5f);
	}

	bool m_selected;
	sf::RectangleShape m_selectedRect;
};
#pragma once

#include <SFML\Graphics.hpp>
#include <Components\ComponentBase.h>

class ComponentCollidable : public ComponentBase
{
public:
	ComponentCollidable(ComponentType type, SystemType owningSystem)
		: ComponentBase(type, owningSystem)
	{
	/*	m_AABB.width = 16;
		m_AABB.height = 16;*/
	}

	sf::FloatRect m_AABB;
};
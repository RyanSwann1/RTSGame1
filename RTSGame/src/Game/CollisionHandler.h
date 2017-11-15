#pragma once

#include <SFML\Graphics.hpp>

namespace CollisionHandler
{
	bool isEntityAtPosition(const sf::FloatRect& entityAABB, int entityID);
	bool isEntityAtPosition(const sf::Vector2f& position);
}
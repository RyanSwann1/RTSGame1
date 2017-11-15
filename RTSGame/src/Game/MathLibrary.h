#pragma once

#include <SFML\Graphics.hpp>

enum class Direction;
namespace MathLibrary
{
	Direction getDirectionFromBetweenPoints(const sf::Vector2f& pointA, const sf::Vector2f& pointB);
	int getDistanceBetweenPoints(const sf::Vector2f& pointA, const sf::Vector2f& pointB);
}
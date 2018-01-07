#pragma once

#include <SFML\Graphics.hpp>

class Graph;
namespace Frontier
{
	void createFrontier(const sf::Vector2f& startingPosition, Graph& graph, int nodeAmount = 0);
}
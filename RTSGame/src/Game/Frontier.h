#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include <Game\Point.h>

class Frontier
{
public:
	Frontier();
	Frontier(const Frontier&) = delete;
	Frontier& operator=(const Frontier&) = delete;
	Frontier(Frontier&&) = delete;
	Frontier&& operator=(Frontier&&) = delete;

	void createFrontier(const sf::Vector2f& startingPosition, int nodeAmount);

private:
	std::vector<Point> m_frontier;

	void assignNextPosition(const sf::Vector2f& startingPosition);
};
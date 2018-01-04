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


	void add(const sf::Vector2f& position);

private:
	std::vector<Point> m_frontier;
	int m_ID;

};
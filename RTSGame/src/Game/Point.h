#pragma once

#include <SFML\Graphics.hpp>

class Point
{
public:
	Point(const sf::Vector2f& position, int ID, int cameFromID)
		: m_position(position),
		m_ID(ID),
		m_cameFromID(cameFromID)
	{}

	const sf::Vector2f m_position;
	const int m_ID;
	const int m_cameFromID;
};
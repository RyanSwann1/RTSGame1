#pragma once

#include <string>
#include <utility>
#include <SFML\Graphics.hpp>

class EntityToSpawn
{
public:
	EntityToSpawn(const sf::Vector2f& startingPosition, std::string&& name)
		: m_startingPosition(startingPosition),
		m_name(std::move(name))
	{}

	const sf::Vector2f m_startingPosition;
	std::string m_name;
};
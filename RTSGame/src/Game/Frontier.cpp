#include <Game\Frontier.h>
#include <Game\CollisionHandler.h>
#include <math.h>

Frontier::Frontier()
	: m_frontier()
{}

void Frontier::createFrontier(const sf::Vector2f & startingPosition, int nodeAmount)
{
	sf::Vector2f nextPosition = startingPosition;
	while (m_frontier.size() < nodeAmount)
	{

	}
}

void Frontier::assignNextPosition(const sf::Vector2f & startingPosition)
{
	const int searchRadius = 1;
	const auto origin = sf::Vector2f(std::floor(startingPosition.x / 16), std::floor(startingPosition.y / 16));
	//x
	for (int x = origin.x - searchRadius; x <= origin.x + searchRadius; ++x)
	{
	
	}
	
	//y
	for(int y = origin.y - searchRadius; y <= origin.y + searchRadius; ++y)
	{

	}

}
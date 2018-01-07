#include <Game\Frontier.h>
#include <vector>
#include <Game\Point.h>
#include <Game\CollisionHandler.h>
#include <Game\Graph.h>

void addNeighbouringPointsToFrontier(const sf::Vector2f& startingPosition, Graph& graph);

void Frontier::createFrontier(const sf::Vector2f & startingPosition, Graph & graph, int nodeAmount)
{
	std::vector<Point> frontier;
	sf::Vector2f nextPosition = startingPosition;
	while (frontier.size() < nodeAmount)
	{
		
	}
}

void addNeighbouringPointsToFrontier(const sf::Vector2f & startingPosition, Graph & graph)
{
	const int searchRadius = 1;
	//x
	for (int x = startingPosition.x - searchRadius; x < startingPosition.x + searchRadius; ++x)
	{
		if (graph.isOnGraph(sf::Vector2f(x, startingPosition.y)))
		{
			continue;
		}

		if(CollisionHandler::isEntityAtPosition()
	}

	//y
	for (int y = startingPosition.y - searchRadius; y < startingPosition.y + searchRadius; ++y)
	{

	}
}
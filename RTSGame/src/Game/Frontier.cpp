#include <Game\Frontier.h>
#include <deque>
#include <Game\Point.h>
#include <Game\CollisionHandler.h>
#include <Game\Graph.h>

void addNeighbouringPointsToFrontier(const sf::Vector2f& startingPosition, Graph& graph, std::deque<Point>& frontier, int& pointID);
void addPoint(const sf::Vector2f& position, Graph& graph, std::deque<Point>& frontier, int& pointID);

void Frontier::createFrontier(const sf::Vector2f & startingPosition, Graph & graph, int maxGraphSize)
{
	std::deque<Point> frontier;
	sf::Vector2f nextPosition = startingPosition;
	int pointID = 0;
	while (frontier.size() < maxGraphSize)
	{
		addNeighbouringPointsToFrontier(startingPosition, graph, frontier, pointID);
		frontier.pop_front();
	}
}

void addNeighbouringPointsToFrontier(const sf::Vector2f & startingPosition, Graph & graph, std::deque<Point>& frontier, int& pointID)
{
	const int searchRadius = 1;
	//x
	for (int x = startingPosition.x - searchRadius; x < startingPosition.x + searchRadius; ++x)
	{
		const auto position = sf::Vector2f(x, startingPosition.y);
		if (graph.isOnGraph(position) &&
			CollisionHandler::isEntityAtPosition(position))
		{
			continue;
		}

		addPoint(sf::Vector2f(x, startingPosition.y), graph, frontier, pointID);
	}

	//y
	for (int y = startingPosition.y - searchRadius; y < startingPosition.y + searchRadius; ++y)
	{
		const auto position = sf::Vector2f(startingPosition.x, y);
		if (graph.isOnGraph(position) && 
			CollisionHandler::isEntityAtPosition(position))
		{
			continue;
		}

		addPoint(sf::Vector2f(startingPosition.x, y), graph, frontier, pointID);
	}
}

void addPoint(const sf::Vector2f& position, Graph & graph, std::deque<Point>& frontier, int& pointID)
{
	int cameFromPointID = 0;
	(frontier.size() > 0 ? cameFromPointID = static_cast<int>(frontier.size()) : cameFromPointID = 0);
	graph.addToGraph(position);
	frontier.emplace_back(position, pointID, cameFromPointID);
}
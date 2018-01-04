#include <Entities\MovementGraph.h>
#include <Game\MathLibrary.h>
#include <assert.h>
#include <Game\DebugOverlay.h>
#include <Game\Direction.h>
#include <Entities\Entity.h>
#include <Components\ComponentMovable.h>
#include <Components\ComponentPosition.h>
#include <Components\ComponentCollidable.h>
#include <Components\ComponentAIMovement.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\EntityManager.h>
#include <Managers\SystemManager.h>
#include <Systems\SystemDirectMessagePosition.h>
#include <Game\CollisionHandler.h>
#include <iostream>
#include <Game\Utilities.h>

//Graph Construction
void constructGraph(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, Graph& graph, int entityID);
sf::Vector2f getNextPositionForGraph(const sf::Vector2f & position, const sf::Vector2f& targetPosition, Graph& graph, int entityID);

void constructGraph(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, Graph& graph, int entityID)
{
	graph.clearGraph();

	sf::Vector2f nextPosition = startingPosition;
	while (nextPosition != targetPosition)
	{
		nextPosition = getNextPositionForGraph(nextPosition, targetPosition, graph, entityID);
		graph.addToGraph(nextPosition);
	}
}

sf::Vector2f getNextPositionForGraph(const sf::Vector2f & position, const sf::Vector2f& targetPosition, Graph& graph, int entityID)
{
	const int searchRadius = 1;
	const int maxRow = 3;
	int y = position.y - 1;
	sf::Vector2f furthestPoint;
	float furthestDistance = FLT_MAX;
	for (int row = 0; row < maxRow; ++row)
	{
		for (int x = position.x - searchRadius; x <= position.x + searchRadius; ++x)
		{
			const float currentDistance = MathLibrary::getDistanceBetweenPoints(sf::Vector2f(x, y), targetPosition);
			if (!graph.isOnGraph(sf::Vector2f(x, y))
				&& !CollisionHandler::isEntityAtPosition(sf::Vector2f(x * 16, y * 16), entityID)
				&& currentDistance < furthestDistance)
			{
				furthestDistance = currentDistance;
				furthestPoint = sf::Vector2f(x, y);
			}
		}
		++y;
	}

	return furthestPoint;
}

//Destination
MovementGraph::Destination::Destination()
	: m_destination(sf::Vector2f(), sf::Vector2f(16, 16))
{}

const sf::FloatRect & MovementGraph::Destination::getDestination() const
{
	return m_destination;
}

sf::Vector2f MovementGraph::Destination::getPosition() const
{
	return sf::Vector2f(m_destination.left, m_destination.top);
}

void MovementGraph::Destination::setPosition(const sf::Vector2f& newPosition)
{
	m_destination.left = newPosition.x;
	m_destination.top = newPosition.y;
}

MovementGraph::TargetPosition::TargetPosition()
	: m_reachedTargetPosition(false),
	m_targetPosition()
{
}

//Target Position
bool MovementGraph::TargetPosition::isTargetPositionReached() const
{
	return m_reachedTargetPosition;
}

const sf::Vector2f & MovementGraph::TargetPosition::getTargetPosition() const
{
	return m_targetPosition;
}

void MovementGraph::TargetPosition::reachedTargetPosition()
{
	m_reachedTargetPosition = true;
}

void MovementGraph::TargetPosition::setTargetPosition(const sf::Vector2f & newPosition)
{
	m_targetPosition = newPosition;
	m_reachedTargetPosition = false;
}

void MovementGraph::TargetPosition::reassignTargetToNeighbouringPosition(const sf::Vector2f & position, Graph& graph, Destination& destination, int entityID)
{
	//const auto& componentPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
	//const auto& position = graph.getGraph().back()->m_position;
	const int frontierSearchRadius = 1;
	const int maxRow = 3;
	const auto targetPos = sf::Vector2f(std::floor(position.x / 16), std::floor(position.y / 16));
	int y = targetPos.y - 1;
	int currentNode = 0;
	bool correctNodeFound = false;
	const int maxNode = 8;
	while (!correctNodeFound)
	{
		const int targetNode = Utilities::getRandomNumber(0, maxNode);
		for (int row = 0; row < maxRow && !correctNodeFound; ++row)
		{
			for (int x = targetPos.x - frontierSearchRadius; x <= targetPos.x + frontierSearchRadius; ++x)
			{
				std::cout << targetNode << "\n";
				std::cout << currentNode << "\n";
				if (currentNode == targetNode && !graph.isOnGraph(sf::Vector2f(x, y))
					&& !CollisionHandler::isEntityAtTile(sf::Vector2f(x * 16, y * 16), entityID))
				{
					m_targetPosition = sf::Vector2f(x * 16, y * 16);
					//m_targetPosition.setTargetPosition(sf::Vector2f(x * 16, y * 16));
					destination.setPosition(sf::Vector2f(x * 16, y * 16));
					graph.addToGraph(sf::Vector2f(x, y));
					correctNodeFound = true;
					break;
				}
				++currentNode;
			}
			++y;
		}
	}
}

//MovementGraph
MovementGraph::MovementGraph()
	: m_graph()
{}

bool MovementGraph::entityReachedTargetPosition() const
{
	return m_targetPosition.isTargetPositionReached();
}

sf::Vector2f MovementGraph::getDestination() const
{
	return sf::Vector2f(m_destination.getDestination().left, m_destination.getDestination().top);
}

void MovementGraph::createGraph(const sf::Vector2f & startingPosition, const sf::Vector2f & targetPosition, std::unique_ptr<Entity>& entity)
{
	//Cannot move to where another entity is
	if (CollisionHandler::isEntityAtPosition(targetPosition, entity->m_ID))
	{
		return;
	}

	const auto targetPos = sf::Vector2f(std::floor(targetPosition.x / 16) * 16, std::floor(targetPosition.y / 16) * 16);
	m_targetPosition.setTargetPosition(targetPos);

	constructGraph(sf::Vector2f(std::floor(startingPosition.x / 16), std::floor(startingPosition.y / 16)), 
		sf::Vector2f(targetPos.x / 16, targetPos.y / 16), m_graph, entity->m_ID);

	changeGraphForEntityCollisions(entity->m_ID);
	assignNewDestination(startingPosition, entity);
}

void MovementGraph::updateDestination(SystemManager& systemManager, EntityManager& entityManager, std::unique_ptr<Entity>& entity)
{
	if (m_graph.getGraph().empty() || 
		!CollisionHandler::isEntityCollidingWithDestination(m_destination.getDestination(), entityManager, entity))
	{
		return;
	}

	//Correct position of entity to match postiion of m_destination
	const auto& correctEntityPosition = m_destination.getPosition();
	if (correctEntityPosition == m_targetPosition.getTargetPosition())
	{
		std::cout << "Reached target Position\n";
		if (CollisionHandler::isEntityAtPosition(correctEntityPosition, entity->m_ID))
		{
			std::cout << "Re-correcting target position due to another entity being in place\n";
			m_targetPosition.reassignTargetToNeighbouringPosition(correctEntityPosition, m_graph, m_destination, entity->m_ID);
		}
		else
		{
			std::cout << "No entity occupying target position\n";
			systemManager.addSystemMessage(SystemMessage(SystemEvent::StopMovement, SystemType::Movable, entity));
			m_targetPosition.reachedTargetPosition();
		}
	}
	else
	{
		std::cout << "Correct the current position of the entity\n";
		systemManager.sendSystemDirectMessagePosition(SystemDirectMessagePosition(correctEntityPosition, entity,
			SystemEvent::CorrectPosition), SystemType::Position);
	}

	m_graph.eraseGraphUntilPosition(correctEntityPosition);
	assignNewDestination(correctEntityPosition, entity);	
}

bool MovementGraph::isEntityOnTargetPosition(const ComponentPosition& componentPosition) const
{
	return (componentPosition.m_position == m_targetPosition.getTargetPosition() ? true : false);
}

void MovementGraph::assignNewDestination(const sf::Vector2f& startingPosition, std::unique_ptr<Entity>& entity)
{
	const auto& graph = m_graph.getGraph();
	if (graph.empty())
	{
		return;
	}

	if (graph.size() == 1)
	{
		m_destination.setPosition(sf::Vector2f(graph.back()->m_position.x * 16, graph.back()->m_position.y * 16));
		return;
	}

	const sf::Vector2f* pointToMoveTo = nullptr;
	const auto destination = sf::Vector2f(graph.cbegin()->get()->m_position.x * 16, graph.cbegin()->get()->m_position.y * 16);
	const auto endPosition = sf::Vector2f(graph.back()->m_position.x * 16, graph.back()->m_position.y * 16);
	
	//If destination is on entirely different axis
	if (startingPosition.x != destination.x && startingPosition.y != destination.y)
	{
		m_destination.setPosition(destination);
	}
	//Find closest destiation on same axis
	else
	{
		for (auto cIter = graph.cbegin() + 1; cIter != graph.cend(); ++cIter)
		{
			if (cIter->get()->m_position.x != startingPosition.x && cIter->get()->m_position.y != startingPosition.y)
			{
				const auto destination = sf::Vector2f(std::floor(m_destination.getPosition().x / 16), std::floor(m_destination.getPosition().y / 16));
				--cIter;
				m_destination.setPosition(sf::Vector2f(cIter->get()->m_position.x * 16, cIter->get()->m_position.y * 16));
				break;
			}
		}
	}
}

void MovementGraph::changeGraphForEntityCollisions(int currentEntityID)
{
	//Temporary hack fix - will change
	const auto& graph = m_graph.getGraph();
	const auto& firstGraphPosition = graph.front()->m_position;
	const auto& lastGraphPosition = graph.back()->m_position;
	if (CollisionHandler::isEntityAtPosition(firstGraphPosition, currentEntityID) || 
		CollisionHandler::isEntityAtPosition(lastGraphPosition, currentEntityID))
	{
		m_graph.clearGraph();
		return;
	}

	//Find nearest point in which movement graph collides with Entity
	const sf::Vector2f* pointA = nullptr;
	for (auto iter = graph.cbegin(); iter != graph.cend(); ++iter)
	{
		const auto& position = iter->get()->m_position;
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(position.x * 16, position.y * 16), currentEntityID))
		{	
			--iter;
			pointA = &iter->get()->m_position;
			break;
		}
	}

	//Find further point in which movement graph collides with entity
	const sf::Vector2f* pointB = nullptr;
	for (int i = graph.size() - 1; i >= 0; --i)
	{
		const auto& position = graph[i]->m_position;
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(position.x * 16, position.y * 16), currentEntityID))
		{
			++i;
			pointB = &graph[i]->m_position;
			break;
		}
	}
}
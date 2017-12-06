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

//Frontier
void createFrontier(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, Graph& graph, int entityID);
sf::Vector2f getNextPositionFromFrontier(const sf::Vector2f & position, const sf::Vector2f& targetPosition, Graph& graph, int entityID);

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

//Frontier
void createFrontier(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, Graph& graph, int entityID)
{
	graph.clearGraph();

	sf::Vector2f nextPosition = startingPosition;
	while (nextPosition != targetPosition)
	{
		nextPosition = getNextPositionFromFrontier(nextPosition, targetPosition, graph, entityID);
		graph.addToGraph(nextPosition);
	}
}

sf::Vector2f getNextPositionFromFrontier(const sf::Vector2f & position, const sf::Vector2f& targetPosition, Graph& graph, int entityID)
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

//MovementGraph
MovementGraph::MovementGraph()
	: m_graph()
{}

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

	createFrontier(sf::Vector2f(std::floor(startingPosition.x / 16), std::floor(startingPosition.y / 16)), 
		sf::Vector2f(std::floor(targetPosition.x / 16), std::floor(targetPosition.y / 16)), m_graph, entity->m_ID);

	changeGraphForEntityCollisions(entity->m_ID);
	assignNewDestination(startingPosition, entity);

	for (const auto& node : m_graph.getGraph())
	{
		DebugOverlay::addShape(sf::Vector2f(node->m_position.x * 16, node->m_position.y * 16));
	}	
}

void MovementGraph::updatePositionToMoveTo(SystemManager& systemManager, EntityManager & entityManager, std::unique_ptr<Entity>& entity)
{
	if (m_graph.getGraph().empty())
	{
		return;
	}
	//Temporary hack - will change
	if (m_destination.getPosition() == m_graph.getGraph().back().get()->m_position && m_graph.getGraph().size() == 1)
	{
		return;
	}

	if (!CollisionHandler::isEntityCollidingWithDestination(m_destination.getDestination(), entityManager, entity))
	{
		return;
	}

	//Correct position of entity to match postiion of m_destination
	const auto& currentEntityPosition = m_destination.getPosition();
	systemManager.sendSystemDirectMessagePosition(SystemDirectMessagePosition(currentEntityPosition, entity, 
		SystemEvent::CorrectPosition), SystemType::Position);

	m_graph.eraseGraphUntilPosition(currentEntityPosition);
	assignNewDestination(currentEntityPosition, entity);	
}

void MovementGraph::assignNewDestination(const sf::Vector2f& startingPosition, std::unique_ptr<Entity>& entity)
{
	if (m_graph.getGraph().empty())
	{
		return;
	}

	const sf::Vector2f* pointToMoveTo = nullptr;
	const auto& graph = m_graph.getGraph();
	const auto positionToMoveTo = sf::Vector2f(graph.cbegin()->get()->m_position.x * 16, graph.cbegin()->get()->m_position.y * 16);
	const auto endPosition = sf::Vector2f(graph.back()->m_position.x * 16, graph.back()->m_position.y * 16);
	
	//If entity at final location of graph
	//Assign new position to move to
	if (CollisionHandler::isEntityAtPosition(endPosition, entity->m_ID))
	{
		assignDestinationAwayFromEntity(entity);
	}

	if (startingPosition.x != positionToMoveTo.x && startingPosition.y != positionToMoveTo.y)
	{
		m_destination.setPosition(positionToMoveTo);
		return;
	}

	for (auto cIter = graph.cbegin() + 1; cIter != graph.cend(); ++cIter)
	{
		if (cIter->get()->m_position.x != startingPosition.x && cIter->get()->m_position.y != startingPosition.y)
		{
			--cIter;
			m_destination.setPosition(sf::Vector2f(cIter->get()->m_position.x * 16, cIter->get()->m_position.y * 16));
			break;
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

	//Make new graph that intersects these two positions wihtout any entity collisions
	//Then insert into existing movment graph to make sure that movmnet graph has no entiyt collisions
}

void MovementGraph::assignDestinationAwayFromEntity(std::unique_ptr<Entity>& entity)
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	const auto& componentPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
	const auto& position = m_graph.getGraph().back()->m_position;

	const int searchRadius = 1;
	const int maxRow = 3;
	int y = position.y - 1;
	for (int row = 0; row < maxRow; ++row)
	{
		for (int x = position.x - searchRadius; x <= position.x + searchRadius; ++x)
		{
			if (!m_graph.isOnGraph(sf::Vector2f(x, y))
				&& !CollisionHandler::isEntityAtPosition(sf::Vector2f(x * 16, y * 16), entity->m_ID))
			{
				m_destination.setPosition(sf::Vector2f(x * 16, y * 16));
				m_graph.addToGraph(sf::Vector2f(x, y));
				return;
			}
		}
		++y;
	}
}
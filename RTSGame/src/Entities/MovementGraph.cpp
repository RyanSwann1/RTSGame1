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

//Point to move to
MovementGraph::PositionToMoveTo::PositionToMoveTo()
	: m_AABB(sf::Vector2f(), sf::Vector2f(16, 16)),
	m_position()
{}

const sf::Vector2f & MovementGraph::PositionToMoveTo::getPosition() const
{
	return m_position;
}

void MovementGraph::PositionToMoveTo::setPosition(const sf::Vector2f & position)
{
	m_position = position;
	m_AABB.left = position.x;
	m_AABB.top = position.y;
}

bool MovementGraph::PositionToMoveTo::intersecting(const sf::FloatRect & entityAABB) const
{
	return m_AABB.intersects(entityAABB);
}

//Frontier
MovementGraph::Frontier::Frontier(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, MovementGraph& movementGraph, int entityID)
{
	std::vector<sf::Vector2f> frontier;
	bool graphComplete = false;
	int tileID = 0;
	sf::Vector2f nextPoint = startingPosition;
	while (!graphComplete)
	{
		addToFrontier(nextPoint, frontier, entityID);
		nextPoint = getNextPoint(targetPosition, frontier, entityID);
		addToGraph(nextPoint, movementGraph, tileID);

		if (nextPoint == targetPosition)
		{
			graphComplete = true;
		}

		//Uneeded points in the frontier
		frontier.clear();
	}
}

void MovementGraph::Frontier::addToFrontier(const sf::Vector2f & position, std::vector<sf::Vector2f>& frontier, int entityID)
{
	const int searchRadius = 1;
	const int maxRow = 3;
	int y = position.y - 1;
	for (int row = 0; row < maxRow; ++row)
	{
		for (int x = position.x - searchRadius; x <= position.x + searchRadius; ++x)
		{
			
			frontier.emplace_back(x, y);
		}

		++y;
	}
}

sf::Vector2f MovementGraph::Frontier::getNextPoint(const sf::Vector2f & targetPosition, const std::vector<sf::Vector2f>& frontier, int entityID) const
{
	int distanceAway = MathLibrary::getDistanceBetweenPoints(frontier.front(), targetPosition);
	const sf::Vector2f* nextPoint = &frontier.front();
	for (auto cIter = frontier.cbegin() + 1; cIter != frontier.cend(); ++cIter)
	{
		const int currentPositionDistanceAway = MathLibrary::getDistanceBetweenPoints((*cIter), targetPosition);
		if (currentPositionDistanceAway < distanceAway)
		{
			distanceAway = currentPositionDistanceAway;
			nextPoint = &(*cIter);	
		}
	}

	return *nextPoint;
}

void MovementGraph::Frontier::addToGraph(const sf::Vector2f & position, MovementGraph& movementGraph, int& tileID) const
{
	int cameFromID = 0;
	(movementGraph.m_graph.empty() ? cameFromID = 0 : cameFromID = movementGraph.m_graph.back()->m_ID);
	++tileID;
	assert(!isOnGraph(position, movementGraph));
	movementGraph.m_graph.emplace_back(std::make_unique<Point>(position, tileID, cameFromID));
}

bool MovementGraph::Frontier::isOnGraph(const sf::Vector2f & position, const MovementGraph& movementGraph) const
{
	auto cIter = std::find_if(movementGraph.m_graph.cbegin(), movementGraph.m_graph.cend(), [&position](const auto& pointOnGraph) { return pointOnGraph->m_position == position; });
	return cIter != movementGraph.m_graph.cend();
}

//MovementGraph
MovementGraph::MovementGraph()
	: m_graph()
{}

const sf::Vector2f & MovementGraph::getPositionToMoveTo() const
{
	return m_positionToMoveTo.getPosition();
}

void MovementGraph::createGraph(const sf::Vector2f & startingPosition, const sf::Vector2f & targetPosition, int entityID)
{
	m_graph.clear();
	Frontier frontier(sf::Vector2f(std::floor(startingPosition.x / 16), std::floor(startingPosition.y / 16)),
		sf::Vector2f(std::floor(targetPosition.x / 16), std::floor(targetPosition.y / 16)), *this, entityID);

	//Scan through existing graph
	checkForEntityCollisions(entityID);

	for (const auto& node : m_graph)
	{
		DebugOverlay::addShape(sf::Vector2f(node->m_position.x * 16, node->m_position.y * 16));
	}

	assignNewPositionToMoveTo(startingPosition);
}

void MovementGraph::updatePositionToMoveTo(SystemManager& systemManager, EntityManager & entityManager, std::unique_ptr<Entity>& entity)
{
	//Check current position of entity
	//If Entity correctly intersects with positionToMoveTo
	//Recalcuate positionToMoveTo
	//Remove nodes from graph up until most recent positionToMoveTo
	//If entity Position == last node in graph
	//Entity has sucessfully reached its target position

	if (m_graph.empty())
	{
		return;
	}
	//Temporary hack - will change
	if (m_positionToMoveTo.getPosition() == m_graph.back().get()->m_position && m_graph.size() == 1)
	{
		return;
	}

	if (!isEntityCollidingWithPositionToMoveTo(entityManager, entity))
	{
		return;
	}

	//Correct position of entity to match postiion of 'm_positionToMoveTo'
	const auto& currentEntityPosition = m_positionToMoveTo.getPosition();
	systemManager.sendSystemDirectMessagePosition(SystemDirectMessagePosition(currentEntityPosition, entity, 
		SystemEvent::CorrectPosition), SystemType::Position);

	eraseGraphUntilPosition(currentEntityPosition);
	assignNewPositionToMoveTo(currentEntityPosition);	
}

bool MovementGraph::isEntityCollidingWithPositionToMoveTo(EntityManager& entityManager, std::unique_ptr<Entity>& entity)
{
	const auto& entityAABB = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity).m_AABB;
	const auto& entityDirection = entityManager.getEntityComponent<ComponentMovable>(ComponentType::Movable, entity).m_movementDirection;
	const auto& entityPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity).m_position;

	if (!m_positionToMoveTo.intersecting(entityAABB))
	{
		return false;
	}

	switch (entityDirection)
	{
	case Direction::Right :
	{
		if (entityPosition.x > m_positionToMoveTo.getPosition().x)
		{
			return true;
		}
		break;
	}
	case Direction::Left :
	{
		if ((entityPosition.x + 16) < m_positionToMoveTo.getPosition().x)
		{
			return true;
		}
		break;
	}
	case Direction::Up :
	{
		if ((entityPosition.y + 16) < m_positionToMoveTo.getPosition().y)
		{
			return true;
		}
		break;
	}
	case Direction::Down :
	{
		if (entityPosition.y > m_positionToMoveTo.getPosition().y)
		{
			return true;
		}
		break;
	}
	}

	return false;
}

void MovementGraph::eraseGraphUntilPosition(const sf::Vector2f & position)
{
	for (auto iter = m_graph.begin(); iter != m_graph.end(); ++iter)
	{
		const auto graphPosition = sf::Vector2f(iter->get()->m_position.x * 16, iter->get()->m_position.y * 16);
	
		DebugOverlay::clearShape(graphPosition);
		iter = m_graph.erase(iter);
		if (graphPosition == position)
		{
			break;
		}
		if (m_graph.empty())
		{
			return;
		}
	}
}

void MovementGraph::assignNewPositionToMoveTo(const sf::Vector2f& startingPosition)
{
	if (m_graph.empty())
	{
		return;
	}
	const sf::Vector2f* pointToMoveTo = nullptr;
	const auto& firstPositionToMoveTo = sf::Vector2f(m_graph.cbegin()->get()->m_position.x * 16, m_graph.cbegin()->get()->m_position.y * 16);

	if (startingPosition.x != firstPositionToMoveTo.x && startingPosition.y != firstPositionToMoveTo.y)
	{
		m_positionToMoveTo.setPosition(firstPositionToMoveTo);
		return;
	}

	for (auto cIter = m_graph.cbegin() + 1; cIter != m_graph.cend(); ++cIter)
	{
		if (cIter->get()->m_position.x != startingPosition.x && cIter->get()->m_position.y != startingPosition.y)
		{
			--cIter;
			m_positionToMoveTo.setPosition(sf::Vector2f(cIter->get()->m_position.x * 16, cIter->get()->m_position.y * 16));
			break;
		}
	}
}

void MovementGraph::checkForEntityCollisions(int currentEntityID)
{
	//Scan through graph to see if any entities - excluding self - are in the way
	//Once found atempt to re direct path around entity
	const auto& targetPosition = m_graph.back().get()->m_position;
	for (auto& iter = m_graph.begin(); iter != m_graph.end(); ++iter)
	{
		const auto& currentPosition = iter->get()->m_position;
		if(CollisionHandler::isEntityAtPosition(sf::Vector2f(currentPosition.x * 16, currentPosition.y * 16), currentEntityID))
		{ 
			std::vector<sf::Vector2f> frontier;
			const int searchRadius = 1;
			const int maxRow = 3;
			int y = currentPosition.y - 1;
			for (int row = 0; row < maxRow; ++row)
			{
				for (int x = currentPosition.x - searchRadius; x <= currentPosition.x + searchRadius; ++x)
				{
					if (!CollisionHandler::isEntityAtPosition(sf::Vector2f(x * 16, y * 16), currentEntityID))
					{
						frontier.emplace_back(x, y);
					}
				}

				++y;
			}

			if (frontier.empty())
			{
				continue;
			}
			int distanceFromTarget = MathLibrary::getDistanceBetweenPoints(sf::Vector2f(frontier.front().x, frontier.front().y),
				sf::Vector2f(targetPosition.x, targetPosition.y));
			const sf::Vector2f* nextPoint = nullptr;
			for (const auto& frontierPosition : frontier)
			{
				const int newDistance = MathLibrary::getDistanceBetweenPoints(sf::Vector2f(frontierPosition.x, frontierPosition.y),
					sf::Vector2f(targetPosition.x, targetPosition.y));
				if (newDistance < distanceFromTarget)
				{
					distanceFromTarget = newDistance;
					nextPoint = &frontierPosition;
				}
			}

			assert(nextPoint);
			iter->get()->m_position = *nextPoint;
		}
	}
}
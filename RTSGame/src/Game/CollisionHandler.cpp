#include <Game\CollisionHandler.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Components\ComponentCollidable.h>
#include <Components\ComponentPosition.h>
#include <Components\ComponentMovable.h>
#include <iostream>

bool CollisionHandler::isEntityAtPosition(const sf::FloatRect& entityAABB, int entityID)
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		//Don't check the same entity for collision
		const auto& componentCollidable = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity);
		if (entity->m_ID != entityID && componentCollidable.m_AABB.intersects(entityAABB))
		{
			return true;
		}
	}

	return false;
}

bool CollisionHandler::isEntityAtPosition(const sf::Vector2f & position, int entityID)
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	const auto AABB = sf::FloatRect(position.x, position.y, 16, 16);
	for (auto& entity : entityManager.getEntities())
	{
		//Don't check same entity for collision
		const auto& componentCollidable = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity);
		//std::cout << "x: " << componentCollidable.m_AABB.left << "\n";
		//std::cout << "y: " << componentCollidable.m_AABB.top << "\n";
		//std::cout << componentCollidable.
		if (entityID != entity->m_ID && componentCollidable.m_AABB.intersects(AABB))
		{
			return true;
		}
	}

	return false;
}

bool CollisionHandler::isEntityCollidingWithDestination(const sf::FloatRect& destination, EntityManager & entityManager, std::unique_ptr<Entity>& entity)
{
	const auto& entityAABB = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity).m_AABB;
	const auto& entityPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity).m_position;
	if (!destination.intersects(entityAABB))
	{
		return false;
	}

	const sf::Vector2f destinationPosition(destination.left, destination.top);
	switch (entityManager.getEntityComponent<ComponentMovable>(ComponentType::Movable, entity).m_movementDirection)
	{
	case Direction::Right:
	{
		if (entityPosition.x > destinationPosition.x)
		{
			return true;
		}
		break;
	}
	case Direction::Left:
	{
		if ((entityPosition.x + 16) < destinationPosition.x)
		{
			return true;
		}
		break;
	}
	case Direction::Up:
	{
		if ((entityPosition.y + 16) < destinationPosition.y)
		{
			return true;
		}
		break;
	}
	case Direction::Down:
	{
		if (entityPosition.y > destinationPosition.y)
		{
			return true;
		}
		break;
	}
	}

	return false;
}

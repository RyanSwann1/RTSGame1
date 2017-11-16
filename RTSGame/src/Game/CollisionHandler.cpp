#include <Game\CollisionHandler.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Components\ComponentCollidable.h>

bool CollisionHandler::isEntityAtPosition(const sf::FloatRect& entityAABB, int entityID)
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (entity->m_ID == entityID)
		{
			continue;
		}

		const auto& componentCollidable = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity);
		if (componentCollidable.m_AABB.intersects(entityAABB))
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
		if (entityID == entity->m_ID)
		{
			continue;
		}

		const auto& componentCollidable = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity);
		if (componentCollidable.m_AABB.intersects(AABB))
		{
			return true;
		}
	}

	return false;
}
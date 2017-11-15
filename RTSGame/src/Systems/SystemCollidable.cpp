#include <Systems\SystemCollidable.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Components\ComponentCollidable.h>
#include <Components\ComponentPosition.h>

SystemCollidable::SystemCollidable(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent)
	: SystemBase(systemManager, systemType, requiredComponent)
{}

void SystemCollidable::update() const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity))
		{
			continue;
		}

		auto& collidableComponent = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity);
		const auto& positionComponent = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
		collidableComponent.m_AABB.left = positionComponent.m_position.x;
		collidableComponent.m_AABB.top = positionComponent.m_position.y;
	}
}
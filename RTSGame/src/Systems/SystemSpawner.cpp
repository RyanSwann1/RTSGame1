#include <Systems\SystemSpawner.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\EntityManager.h>
#include <Components\ComponentSpawner.h>
#include <Locators\FrameTimerLocator.h>
#include <Game\FrameTimer.h>
#include <Game\Graph.h>
#include <Game\Frontier.h>

SystemSpawner::SystemSpawner(SystemManager & systemManager, SystemType systemType, ComponentType requiredComponent)
	: SystemBase(systemManager, systemType, requiredComponent)
{}

void SystemSpawner::update() const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity))
		{
			continue;
		}
		
		auto& componentSpawner = entityManager.getEntityComponent<ComponentSpawner>(ComponentType::Spawner, entity);
		updateComponentSpawnTimer(componentSpawner);
		if (!componentSpawner.m_entitySpawnQueue.empty())
		{
			spawnEntity(entity, componentSpawner, entityManager);
		}
	}
}

void SystemSpawner::spawnEntity(std::unique_ptr<Entity>& entity, ComponentSpawner& componentSpawner, EntityManager& entityManager) const
{
	if (componentSpawner.m_spawnTimer.isExpired())
	{
		auto& entityToSpawn = componentSpawner.m_entitySpawnQueue.back();
		//Assign the starting position from here via a Frontier/Graph
		componentSpawner.m_spawnTimer.reset();
	}
}

void SystemSpawner::updateComponentSpawnTimer(ComponentSpawner & componentSpawner) const
{
	componentSpawner.m_spawnTimer.update(FrameTimerLocator::getFrameTimer().getDeltaTime());
}
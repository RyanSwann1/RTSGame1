#pragma once

#include <Components\ComponentBase.h>
#include <vector>
#include <Entities\EntityToSpawn.h>
#include <Game\Timer.h>

class ComponentSpawner : public ComponentBase
{
public:
	ComponentSpawner(ComponentType componentType, SystemType owningSystem)
		: ComponentBase(componentType, owningSystem),
		m_entitySpawnQueue(),
		m_spawnTimer(3.5f)
	{}
	ComponentSpawner(const ComponentSpawner&) = delete;
	ComponentSpawner& operator=(const ComponentSpawner&) = delete;
	ComponentSpawner(ComponentSpawner&&) = delete;
	ComponentSpawner&& operator=(ComponentSpawner&&) = delete;

	std::vector<EntityToSpawn> m_entitySpawnQueue;
	Timer m_spawnTimer;
};
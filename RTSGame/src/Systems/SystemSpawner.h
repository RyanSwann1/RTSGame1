#pragma once

#include <Systems\SystemBase.h>
#include <memory>

class ComponentSpawner;
class SystemSpawner : public SystemBase
{
public:
	SystemSpawner(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent);
	SystemSpawner(const SystemSpawner&) = delete;
	SystemSpawner& operator=(const SystemSpawner&) = delete;
	SystemSpawner(SystemSpawner&&) = delete;
	SystemSpawner&& operator=(SystemSpawner&&) = delete;

	void update() const override;

private:
	void spawnEntity(std::unique_ptr<Entity>& entity, ComponentSpawner& componentSpawner) const;
	void updateComponentSpawnTimer(ComponentSpawner& componentSpawner) const;
};
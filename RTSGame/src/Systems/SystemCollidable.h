#pragma once

#include <Systems\SystemBase.h>
class SystemCollidable : public SystemBase
{
public:
	SystemCollidable(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent);
	SystemCollidable(const SystemCollidable&) = delete;
	SystemCollidable& operator=(const SystemCollidable&) = delete;
	SystemCollidable(SystemCollidable&&) = delete;
	SystemCollidable&& operator=(SystemCollidable&&) = delete;

	void update() const override;
};
#pragma once

#include <Components\ComponentBase.h>

class ComponentBuilding : public ComponentBase
{
public:
	//ComponentBase(ComponentType componentType, SystemType owningSystem)
	ComponentBuilding(ComponentType componentType, SystemType owningSystem)
		: ComponentBase(componentType, owningSystem)
	{}

	ComponentBuilding(ComponentBuilding&) = delete;
	ComponentBuilding& operator=(ComponentBuilding&) = delete;
	ComponentBuilding(ComponentBuilding&&) = delete;
	ComponentBuilding&& operator=(ComponentBuilding) = delete;

private:

};
#pragma once

#include <Systems\SystemType.h>
#include <Components\ComponentType.h>

class ComponentBase
{
public:
	ComponentBase(ComponentType componentType, SystemType owningSystem)
		: m_type(componentType),
		m_owningSystem(owningSystem)
	{}
	ComponentBase(const ComponentBase&) = delete;
	ComponentBase& operator=(const ComponentBase&) = delete;
	ComponentBase(ComponentBase&&) = delete;
	ComponentBase&& operator=(ComponentBase&&) = delete;
	virtual ~ComponentBase() {}

	const ComponentType m_type;
	const SystemType m_owningSystem;
};
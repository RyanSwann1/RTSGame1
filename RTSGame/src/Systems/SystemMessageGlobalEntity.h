#pragma once

#include <Systems\SystemEvent.h>
#include <Systems\SystemType.h>

class SystemMessageGlobalEntity
{
public:
	SystemMessageGlobalEntity(SystemEvent systemEvent, SystemType systemType)
		: m_systemEvent(systemEvent),
		m_systemType(systemType)
	{}

	const SystemEvent m_systemEvent;
	const SystemType m_systemType;
};
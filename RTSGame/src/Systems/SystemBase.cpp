#include "SystemBase.h"
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Systems\SystemMessage.h>
#include <assert.h>
#include <Systems\SystemDirectMessagePosition.h>

SystemBase::SystemBase(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent)
	:  m_type(systemType),
	m_requiredComponent(requiredComponent),
	m_systemManager(systemManager)
{
}

bool SystemBase::entityFitsRequirements(const std::unique_ptr<Entity>& entity) const
{
	const auto& entityManager = EntityManagerLocator::getEntityManager();
	const bool componentFound = entityManager.entityHasComponent(entity, m_requiredComponent);
	return componentFound;
}

bool SystemBase::entityFitsRequirements(const std::unique_ptr<Entity>& entity, ComponentType componentType) const
{
	const auto& entityManager = EntityManagerLocator::getEntityManager();
	const bool componentFound = entityManager.entityHasComponent(entity, componentType);
	assert(componentFound);
	return componentFound;
}

void SystemBase::onSystemMessage(const SystemMessage& systemMessage) const
{
	onSystemEvent(systemMessage.m_systemEvent, systemMessage.m_entity);
}
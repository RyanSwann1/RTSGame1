#include <Managers\SystemManager.h>
#include <Systems\SystemDrawable.h>
#include <Systems\SystemSelectable.h>
#include <Systems\SystemCollidable.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\EntityManager.h>
#include <Systems\SystemMovable.h>
#include <Systems\SystemAIMovement.h>
#include <Systems\SystemMessageGlobalEntity.h>
#include <Systems\SystemPosition.h>

SystemManager::SystemManager()
	: m_systems(),
	m_systemMessages()
{
	addSystem<SystemDrawable>(SystemType::Drawable, ComponentType::Drawable);
	addSystem<SystemSelectable>(SystemType::Selectable, ComponentType::Selectable);
	addSystem<SystemCollidable>(SystemType::Collidable, ComponentType::Collidable);
	addSystem<SystemMovable>(SystemType::Movable, ComponentType::Movable);
	addSystem<SystemAIMovement>(SystemType::AIMovemement, ComponentType::AIMovement);
	addSystem<SystemPosition>(SystemType::Position, ComponentType::Position);
}

void SystemManager::sendSystemDirectMessagePosition(const SystemDirectMessagePosition & systemMessage, SystemType systemType) const
{
	getSystem<SystemAIMovement>(systemType).onSystemDirectMessagePosition(systemMessage);
}

void SystemManager::sendGlobalEntitySystemMessage(const SystemMessageGlobalEntity & systemMessageGlobalEntity)
{
	getSystemByType(systemMessageGlobalEntity.m_systemType)->onSystemGlobalEntityMessage(systemMessageGlobalEntity.m_systemEvent);
}

void SystemManager::addSystemMessage(const SystemMessage & systemMessage)
{
	m_systemMessages.push_back(systemMessage);
}

void SystemManager::draw(sf::RenderWindow & window) const
{	
	getSystem<SystemDrawable>(SystemType::Drawable).draw(window);
	getSystem<SystemSelectable>(SystemType::Selectable).draw(window);
}

void SystemManager::update()
{
	for (auto& system : m_systems)
	{
		assert(system.get());
		system->update();
	}

	handleEvents();
}

std::unique_ptr<SystemBase>& SystemManager::getSystemByType(SystemType systemType)
{
	return m_systems[static_cast<int>(systemType)];
}

void SystemManager::handleEvents()
{
	while (!m_systemMessages.empty())
	{
		const auto& systemMessage = m_systemMessages.front();
		sendMessageToSystems(systemMessage);
		m_systemMessages.pop_front();
	} 
}

void SystemManager::sendMessageToSystems(const SystemMessage & systemMessage)
{
	if (systemMessage.m_globalSystemMessage)
	{
		for (const auto& system : m_systems)
		{
			system->onSystemMessage(systemMessage);
		}
	}
	else
	{
		getSystemByType(systemMessage.m_systemType)->onSystemMessage(systemMessage);
	}
}
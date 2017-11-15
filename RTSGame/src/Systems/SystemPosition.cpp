#include <Systems\SystemPosition.h>
#include <Systems\SystemDirectMessagePosition.h>
#include <Components\ComponentPosition.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>

SystemPosition::SystemPosition(SystemManager & systemManager, SystemType type, ComponentType requiredComponent)
	: SystemBase(systemManager, type, requiredComponent)
{}

void SystemPosition::onSystemDirectMessagePosition(const SystemDirectMessagePosition & systemMessage) const
{
	switch (systemMessage.m_systemEvent)
	{
		case SystemEvent::CorrectPosition :
		{
			setEntityPosition(systemMessage.m_message, systemMessage.m_entity);
			break;
		}
	}
}

void SystemPosition::setEntityPosition(const sf::Vector2f & position, std::unique_ptr<Entity>& entity) const
{
	auto& componentPosition = EntityManagerLocator::getEntityManager().getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
	componentPosition.m_position = position;
}
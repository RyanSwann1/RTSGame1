#include <Systems\SystemMovable.h>
#include <Systems\SystemEvent.h>
#include <Managers\EntityManager.h>
#include <Managers\SystemManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Components\ComponentMovable.h>
#include <Components\ComponentPosition.h>
#include <Locators\FrameTimerLocator.h>
#include <Systems\SystemDirectMessagePosition.h>
#include <Game\FrameTimer.h>
#include <iostream>

SystemMovable::SystemMovable(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent)
	: SystemBase(systemManager, systemType, requiredComponent)
{}

void SystemMovable::update() const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity) || !SystemBase::entityFitsRequirements(entity, ComponentType::Position))
		{
			continue;
		}

		auto& componentPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
		auto& componentMovable = entityManager.getEntityComponent<ComponentMovable>(ComponentType::Movable, entity);
		moveEntity(componentMovable, componentPosition);
	}
}

void SystemMovable::onSystemEvent(SystemEvent systemEvent, std::unique_ptr<Entity>& entity) const
{
	switch (systemEvent)
	{
	case SystemEvent::MoveLeft :
	{
		moveInDirection(systemEvent, entity);
		break;
	}
	case SystemEvent::MoveRight :
	{
		moveInDirection(systemEvent, entity);
		break;
	}
	case SystemEvent::MoveUp :
	{
		moveInDirection(systemEvent, entity);
		break;
	}
	case SystemEvent::MoveDown :
	{
		moveInDirection(systemEvent, entity);
		break;
	}
	case SystemEvent::StopMovement :
	{
		moveInDirection(systemEvent, entity);
		break;
	}
	case SystemEvent::MoveDownLeft :
	{
		moveInDirection(systemEvent, entity);
		break;
	}
	case SystemEvent::MoveDownRight :
	{
		moveInDirection(systemEvent, entity);
		break;
	}
	case SystemEvent::MoveUpRight :
	{
		moveInDirection(systemEvent, entity);
		break;
	}
	case SystemEvent::MoveUpLeft :
	{
		moveInDirection(systemEvent, entity);
		break;
	}
	}
}

void SystemMovable::moveInDirection(SystemEvent systemEvent, std::unique_ptr<Entity>& entity) const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	auto& componentMovable = entityManager.getEntityComponent<ComponentMovable>(ComponentType::Movable, entity);
	
	const auto& speed = componentMovable.m_speed;
	auto& velocity = componentMovable.m_velocity;
	auto& movementDirection = componentMovable.m_movementDirection;
	switch (systemEvent)
	{
	case SystemEvent::MoveLeft:
	{
		setEntityVelocity(sf::Vector2f(-speed.x, 0), velocity);
		movementDirection = Direction::Left;
		break;
	}
	case SystemEvent::MoveRight:
	{
		setEntityVelocity(sf::Vector2f(speed.x, 0), velocity);
		movementDirection = Direction::Right;
		break;
	}
	case SystemEvent::MoveUp:
	{
		setEntityVelocity(sf::Vector2f(0, -speed.y), velocity);
		movementDirection = Direction::Up;
		break;
	}
	case SystemEvent::MoveDown:
	{
		setEntityVelocity(sf::Vector2f(0, speed.y), velocity);
		movementDirection = Direction::Down;
		break;
	}
	case SystemEvent::MoveUpRight :
	{
		setEntityVelocity(sf::Vector2f(speed.x, -speed.y), velocity);
		movementDirection = Direction::Right;
		break;
	}
	case SystemEvent::MoveDownRight :
	{
		setEntityVelocity(sf::Vector2f(speed.x, speed.y), velocity);
		movementDirection = Direction::Right;
		break;
	}
	case SystemEvent::MoveDownLeft :
	{
		setEntityVelocity(sf::Vector2f(-speed.x, speed.y), velocity);
		movementDirection = Direction::Left;
		break;
	}
	case SystemEvent::MoveUpLeft : 
	{
		setEntityVelocity(sf::Vector2f(-speed.x, -speed.y), velocity);
		movementDirection = Direction::Left;
		break;
	}
	case SystemEvent::StopMovement :
	{
		resetVelocity(componentMovable);
		movementDirection = Direction::None;
		break;
	}
	}
}

void SystemMovable::moveEntity(const ComponentMovable & componentMovable, ComponentPosition & componentPosition) const
{
	const float deltaTime = FrameTimerLocator::getFrameTimer().getDeltaTime();
	componentPosition.m_position.x += componentMovable.m_velocity.x * deltaTime;
	componentPosition.m_position.y += componentMovable.m_velocity.y * deltaTime;
}

void SystemMovable::resetVelocity(ComponentMovable & componentMovable) const
{
	componentMovable.m_velocity.x = 0;
	componentMovable.m_velocity.y = 0;
}

void SystemMovable::setEntityVelocity(const sf::Vector2f & speed, sf::Vector2f & velocity) const
{
	velocity = speed;
}
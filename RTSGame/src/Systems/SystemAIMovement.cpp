#include <Systems\SystemAIMovement.h>
#include <Components\ComponentAIMovement.h>
#include <Components\ComponentPosition.h>
#include <Components\ComponentMovable.h>
#include <Components\ComponentCollidable.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\SystemManager.h>
#include <Systems\SystemDirectMessagePosition.h>
#include <Game\Direction.h>
#include <math.h>
#include <Game\MathLibrary.h>
#include <Game\CollisionHandler.h>
#include <iostream>

//System AIMovement
SystemAIMovement::SystemAIMovement(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent)
	: SystemBase(systemManager, systemType, requiredComponent)
{}

void SystemAIMovement::update() const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity))
		{
			continue;
		}
		
		updateDestination(entityManager, entity);
		handleEntityMovementDirection(entityManager, entity);
		onEntityReachingTargetPosition(entityManager, entity);
	}
}

void SystemAIMovement::onSystemDirectMessagePosition(const SystemDirectMessagePosition & systemMessage) const
{
	if (!SystemBase::entityFitsRequirements(systemMessage.m_entity))
	{
		return;
	}

	switch (systemMessage.m_systemEvent)
	{
	case SystemEvent::SetMovementTargetPosition :
	{
		setNewMovementTargetPosition(systemMessage.m_message, systemMessage.m_entity);
		break;
	}
	}
}

void SystemAIMovement::setNewMovementTargetPosition(const sf::Vector2f& targetPosition, std::unique_ptr<Entity>& entity) const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	auto& componentAIMovement = entityManager.getEntityComponent<ComponentAIMovement>(ComponentType::AIMovement, entity);
	const auto& componentPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);

	componentAIMovement.m_movementGraph.createGraph(componentPosition.m_position, targetPosition, entity);
}

void SystemAIMovement::handleEntityMovementDirection(const EntityManager& entityManager, std::unique_ptr<Entity>& entity) const
{
	const auto& movementGraph = entityManager.getEntityComponent<ComponentAIMovement>(ComponentType::AIMovement, entity).m_movementGraph;
	if (movementGraph.entityReachedTargetPosition() || movementGraph.getDestination() == sf::Vector2f(0, 0))
	{
		return;
	}

	const auto& componentPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
	const auto moveDirection = MathLibrary::getDirectionFromBetweenPoints(componentPosition.m_position, movementGraph.getDestination());
	std::cout << static_cast<int>(moveDirection) << "\n";
	switch (moveDirection)
	{
	case Direction::Right :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveRight, SystemType::Movable, entity));
		break;
	}
	case Direction::Left :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveLeft, SystemType::Movable, entity));
		break;
	}
	case Direction::Up :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveUp, SystemType::Movable, entity));
		break;
	}
	case Direction::Down :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveDown, SystemType::Movable, entity));
		break;
	}
	case Direction::UpLeft :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveUpLeft, SystemType::Movable, entity));
		break;
	}
	case Direction::UpRight :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveUpRight, SystemType::Movable, entity));
		break;
	}
	case Direction::DownRight :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveDownRight, SystemType::Movable, entity));
		break;
	}
	case Direction::DownLeft :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveDownLeft, SystemType::Movable, entity));
		break;
	}
	}
}

void SystemAIMovement::updateDestination(EntityManager & entityManager, std::unique_ptr<Entity>& entity) const
{
	auto& componentAIMovement = entityManager.getEntityComponent<ComponentAIMovement>(ComponentType::AIMovement, entity);
	componentAIMovement.m_movementGraph.updateDestination(m_systemManager, entityManager, entity);
}

void SystemAIMovement::onEntityReachingTargetPosition(EntityManager & entityManager, std::unique_ptr<Entity>& entity) const
{
	auto& componentAIMovement = entityManager.getEntityComponent<ComponentAIMovement>(ComponentType::AIMovement, entity);
	componentAIMovement.m_movementGraph.onEntityReachingTargetPosition(m_systemManager, entityManager, entity);
}
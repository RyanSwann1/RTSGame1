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
#include <Game\DebugOverlay.h>
#include <Game\MathLibrary.h>

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
		
		updatePositionToMoveTo(entityManager, entity);
		handleEntityMovement(entityManager, entity);
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
		DebugOverlay::clearShapes();
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

	componentAIMovement.m_movementGraph.createGraph(componentPosition.m_position, targetPosition);
}

void SystemAIMovement::handleEntityMovement(const EntityManager& entityManager, std::unique_ptr<Entity>& entity) const
{
	const auto& componentPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
	const auto& positionToMoveTo = entityManager.getEntityComponent<ComponentAIMovement>(ComponentType::AIMovement, entity).m_movementGraph.getPositionToMoveTo();
	const auto moveDirection = MathLibrary::getDirectionFromBetweenPoints(componentPosition.m_position, positionToMoveTo);

	if (positionToMoveTo == sf::Vector2f(0, 0))
	{
		return;
	}

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

void SystemAIMovement::updatePositionToMoveTo(EntityManager & entityManager, std::unique_ptr<Entity>& entity) const
{
	auto& componentAIMovement = entityManager.getEntityComponent<ComponentAIMovement>(ComponentType::AIMovement, entity);
	componentAIMovement.m_movementGraph.updatePositionToMoveTo(m_systemManager, entityManager, entity);
}
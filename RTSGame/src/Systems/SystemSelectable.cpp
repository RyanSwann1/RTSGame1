#include <Systems\SystemSelectable.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Components\ComponentSelectable.h>
#include <Components\ComponentPosition.h>
#include <Systems\SystemEvent.h>

SystemSelectable::SystemSelectable(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent)
	: SystemBase(systemManager, systemType, requiredComponent) 
{}

void SystemSelectable::draw(sf::RenderWindow & window)
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity))
		{
			continue;
		}

		const auto& selectableComponent = entityManager.getEntityComponent<ComponentSelectable>(ComponentType::Selectable, entity);
		if(selectableComponent.m_selected)
		{
			window.draw(selectableComponent.m_selectedRect);
		}
	}
}

void SystemSelectable::update() const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity))
		{
			continue;
		}

		auto& selectableComponent = entityManager.getEntityComponent<ComponentSelectable>(ComponentType::Selectable, entity);
		if (selectableComponent.m_selected)
		{
			const auto& position = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity).m_position;
			selectableComponent.m_selectedRect.setPosition(sf::Vector2f(position.x - 3, position.y - 3));
			
		}
	}
}

void SystemSelectable::onSystemEvent(SystemEvent systemEvent, std::unique_ptr<Entity>& entity) const
{
	switch (systemEvent)
	{
	case SystemEvent::Selected :
	{
		handleSelection(entity);
		break;
	}
	case SystemEvent::Deselected :
	{
		handleSingularEntityDeselection(entity);
		break;
	}
	}
}

void SystemSelectable::onGlobalSystemEvent(SystemEvent systemEvent) const
{
	switch (systemEvent)
	{
	case SystemEvent::Deselected :
	{
		handleGlobalEntitySelection();
		break;
	}
	}
}

void SystemSelectable::handleSelection(std::unique_ptr<Entity>& entity) const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	auto& componentSelectable = entityManager.getEntityComponent<ComponentSelectable>(ComponentType::Selectable, entity);
	
	componentSelectable.m_selected = true;
}

void SystemSelectable::handleSingularEntityDeselection(std::unique_ptr<Entity>& entity) const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	auto& componentSelectable = entityManager.getEntityComponent<ComponentSelectable>(ComponentType::Selectable, entity);

	componentSelectable.m_selected = false;
}

void SystemSelectable::handleGlobalEntitySelection() const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		auto& componentSelectable = entityManager.getEntityComponent<ComponentSelectable>(ComponentType::Selectable, entity);
		componentSelectable.m_selected = false;
	}
}
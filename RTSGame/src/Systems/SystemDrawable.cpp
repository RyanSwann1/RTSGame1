#include <Systems\SystemDrawable.h>
#include <Locators\EntityManagerLocator.h>
#include <Components\ComponentDrawable.h>
#include <Components\ComponentPosition.h>
#include <Managers\EntityManager.h>

SystemDrawable::SystemDrawable(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent) 
	: SystemBase(systemManager, systemType, requiredComponent)
{}

void SystemDrawable::draw(sf::RenderWindow & window) const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity))
		{
			continue;
		}

		const auto& entityRect = entityManager.getEntityComponent<ComponentDrawable>(ComponentType::Drawable, entity).m_rect;
		window.draw(entityRect);
	}
}

void SystemDrawable::update() const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity))
		{
			continue;
		}

		//Update position of the rectangle
		const auto& positionComponent = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
		auto& drawableComponent = entityManager.getEntityComponent<ComponentDrawable>(ComponentType::Drawable, entity);
		drawableComponent.m_rect.setPosition(positionComponent.m_position);
	}
}
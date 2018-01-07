#include "EntitySelector.h"
#include <Managers\EventManager.h>
#include <Window.h>
#include <InputEvent.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Components\ComponentCollidable.h>
#include <Managers\SystemManager.h>
#include <Components\ComponentSelectable.h>
#include <Systems\SystemAIMovement.h>
#include <Systems\SystemDirectMessagePosition.h>
#include <Systems\SystemMessageGlobalEntity.h>
#include <Game\DebugOverlay.h>
#include <math.h>
#include <iostream>
#include <Game\Graph.h>

EntitySelector::EntitySelector(Window& window, EventManager<InputEvent>& eventManager, SystemManager& systemManager)
	: m_window(window),
	m_eventManager(eventManager),
	m_systemManager(systemManager),
	m_leftMouseButtonHeld(false),
	m_rightMouseButtonDown(false),
	m_secondaryLeftMouseButtonActivated(false),
	m_rect(),
	m_rectAABB()
{
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::Green);
	m_rect.setOutlineThickness(1.5f);

	m_eventManager.subscribe(InputEvent::LeftClick, std::bind(&EntitySelector::onMouseButtonLeftDown, this), "OnLeftClick");
	m_eventManager.subscribe(InputEvent::LeftClickReleased, std::bind(&EntitySelector::onMouseButtonLeftRelease, this), "OnLeftRelease");
	m_eventManager.subscribe(InputEvent::RightClick, std::bind(&EntitySelector::onMouseButtonRightDown, this), "OnRightClick");
	m_eventManager.subscribe(InputEvent::RightClickReleased, std::bind(&EntitySelector::onMouseButtonRightRelease, this), "OnRightRelease");
}

EntitySelector::~EntitySelector()
{
	m_eventManager.unsubscribe(InputEvent::LeftClick, "OnLeftClick");
	m_eventManager.unsubscribe(InputEvent::LeftClickReleased, "OnLeftRelease");
	m_eventManager.unsubscribe(InputEvent::RightClick, "OnRightClick");
	m_eventManager.unsubscribe(InputEvent::RightClickReleased, "OnRightRelease");
}

void EntitySelector::update()
{
	if (m_leftMouseButtonHeld)
	{
		const sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window.getWindow());
		m_rect.setSize(sf::Vector2f(mousePosition.x - m_rect.getPosition().x, mousePosition.y - m_rect.getPosition().y));
		m_rectAABB.width = mousePosition.x - m_rectAABB.left;
		m_rectAABB.height = mousePosition.y - m_rectAABB.top;

		handleEntityCollisions();
	}
}

void EntitySelector::draw(sf::RenderWindow & window) const
{
	if (m_leftMouseButtonHeld)
	{
		window.draw(m_rect);
	}
}

void EntitySelector::onMouseButtonLeftDown()
{
	m_leftMouseButtonHeld = true;
	const auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(m_window.getWindow()).x, sf::Mouse::getPosition(m_window.getWindow()).y);
	m_rect.setPosition(mousePosition);
	m_rectAABB.left = mousePosition.x;
	m_rectAABB.top = mousePosition.y; 

	sf::FloatRect clickAABB(mousePosition, sf::Vector2f(16, 16));
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		const auto& componentCollidable = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity);
		if (clickAABB.intersects(componentCollidable.m_AABB))
		{
			m_systemManager.addSystemMessage(SystemMessage(SystemEvent::Selected, SystemType::Selectable, entity));
		}
		else
		{
			m_systemManager.addSystemMessage(SystemMessage(SystemEvent::Deselected, SystemType::Selectable, entity));
		}
	}

	if (m_secondaryLeftMouseButtonActivated)
	{
		m_secondaryLeftMouseButtonActivated = false;
		m_systemManager.sendGlobalEntitySystemMessage(SystemMessageGlobalEntity(SystemEvent::Deselected, SystemType::Selectable));
	}
}

void EntitySelector::onMouseButtonLeftRelease()
{
	m_secondaryLeftMouseButtonActivated = true;
	m_leftMouseButtonHeld = false;
}

void EntitySelector::onMouseButtonRightDown()
{
	m_rightMouseButtonDown = true;
	if (m_secondaryLeftMouseButtonActivated)
	{
		auto& entityManager = EntityManagerLocator::getEntityManager();
		for (auto& entity : entityManager.getEntities())
		{
			const auto& componentSelectable = entityManager.getEntityComponent<ComponentSelectable>(ComponentType::Selectable, entity);
			if (componentSelectable.m_selected)
			{
				const auto mousePosition = sf::Vector2f(std::floor(sf::Mouse::getPosition(m_window.getWindow()).x) / 16,
					std::floor(sf::Mouse::getPosition(m_window.getWindow()).y) / 16);

				m_systemManager.sendSystemDirectMessagePosition(SystemDirectMessagePosition(sf::Vector2f(mousePosition.x * 16, mousePosition.y * 16), 
					entity, SystemEvent::SetMovementTargetPosition), SystemType::AIMovemement);
			}
		}
	}
}

void EntitySelector::onMouseButtonRightRelease()
{
	m_rightMouseButtonDown = false;
}

void EntitySelector::handleEntityCollisions()
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		const auto& componentCollidable = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity);
		if (m_rectAABB.intersects(componentCollidable.m_AABB))
		{
			m_systemManager.addSystemMessage(SystemMessage(SystemEvent::Selected, SystemType::Selectable, entity));
		}
	}
}


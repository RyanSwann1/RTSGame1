#include "BoxSelector.h"
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

BoxSelector::BoxSelector(Window& window, EventManager<InputEvent>& eventManager, SystemManager& systemManager)
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

	m_eventManager.subscribe(InputEvent::LeftClick, std::bind(&BoxSelector::onMouseButtonLeftDown, this), "OnLeftClick");
	m_eventManager.subscribe(InputEvent::LeftClickReleased, std::bind(&BoxSelector::onMouseButtonLeftRelease, this), "OnLeftRelease");
	m_eventManager.subscribe(InputEvent::RightClick, std::bind(&BoxSelector::onMouseButtonRightDown, this), "OnRightClick");
	m_eventManager.subscribe(InputEvent::RightClickReleased, std::bind(&BoxSelector::onMouseButtonRightRelease, this), "OnRightRelease");
}

BoxSelector::~BoxSelector()
{
	m_eventManager.unsubscribe(InputEvent::LeftClick, "OnLeftClick");
	m_eventManager.unsubscribe(InputEvent::LeftClickReleased, "OnLeftRelease");
	m_eventManager.unsubscribe(InputEvent::RightClick, "OnRightClick");
	m_eventManager.unsubscribe(InputEvent::RightClickReleased, "OnRightRelease");
}

void BoxSelector::update()
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

void BoxSelector::draw(sf::RenderWindow & window) const
{
	if (m_leftMouseButtonHeld)
	{
		window.draw(m_rect);
	}
}

void BoxSelector::onMouseButtonLeftDown()
{
	m_leftMouseButtonHeld = true;
	const auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(m_window.getWindow()).x, sf::Mouse::getPosition(m_window.getWindow()).x);
	m_rect.setPosition(mousePosition);
	m_rectAABB.left = mousePosition.x;
	m_rectAABB.top = mousePosition.y; 

	if (m_secondaryLeftMouseButtonActivated)
	{
		m_secondaryLeftMouseButtonActivated = false;
		m_systemManager.sendGlobalEntitySystemMessage(SystemMessageGlobalEntity(SystemEvent::Deselected, SystemType::Selectable));
	}
}

void BoxSelector::onMouseButtonLeftRelease()
{
	m_secondaryLeftMouseButtonActivated = true;
	m_leftMouseButtonHeld = false;
}

void BoxSelector::onMouseButtonRightDown()
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
				const auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(m_window.getWindow()).x, sf::Mouse::getPosition(m_window.getWindow()).y);
				m_systemManager.sendSystemDirectMessagePosition(SystemDirectMessagePosition(mousePosition, 
					entity, SystemEvent::SetMovementTargetPosition), SystemType::AIMovemement);
			}
		}
	}
}

void BoxSelector::onMouseButtonRightRelease()
{
	m_rightMouseButtonDown = false;
}

void BoxSelector::handleEntityCollisions()
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		const auto& componentCollidable = entityManager.getEntityComponent<ComponentCollidable>(ComponentType::Collidable, entity);
		if (m_rectAABB.intersects(componentCollidable.m_AABB))
		{
			m_systemManager.addSystemMessage(SystemMessage(SystemEvent::Selected, SystemType::Selectable, entity));
		}
		else
		{
			m_systemManager.addSystemMessage(SystemMessage(SystemEvent::Deselected, SystemType::Selectable, entity));
		}
	}
}
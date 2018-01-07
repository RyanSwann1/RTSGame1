#include "Game.h"
#include <Locators\FrameTimerLocator.h>
#include <Game\DebugOverlay.h>

Game::Game()
	: m_frameTimer(),
	m_inputEventManager(),
	m_window(sf::Vector2i(750, 750), "SFML_WINDOW", m_inputEventManager),
	m_entitySelector(m_window, m_inputEventManager, m_systemManager),
	m_entityManager(),
	m_systemManager()
{
	m_entityManager.addEntity("MarineSpawner", sf::Vector2f(240, 240));
}

bool Game::isRunning() const
{
	return m_window.isOpen();
}

void Game::update()
{
	m_entitySelector.update();
	m_frameTimer.update();
	m_systemManager.update();
	m_window.update();
}

void Game::draw()
{
	m_window.clearWindow();
	m_systemManager.draw(m_window.getWindow());
	DebugOverlay::draw(m_window.getWindow());
	m_entitySelector.draw(m_window.getWindow());
	m_window.display();
}

void Game::lateUpdate()
{
	m_frameTimer.update();
}
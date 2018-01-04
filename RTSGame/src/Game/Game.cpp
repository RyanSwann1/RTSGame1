#include "Game.h"
#include <Locators\FrameTimerLocator.h>
#include <Game\DebugOverlay.h>

Game::Game()
	: m_frameTimer(),
	m_inputEventManager(),
	m_window(sf::Vector2i(750, 750), "SFML_WINDOW", m_inputEventManager),
	m_boxSelector(m_window, m_inputEventManager, m_systemManager),
	m_entityManager(),
	m_systemManager()
{
	m_entityManager.addEntity("Marine", sf::Vector2f(192, 192));
	m_entityManager.addEntity("Marine", sf::Vector2f(128, 128));
	//m_entityManager.addEntity("Marine", sf::Vector2f(144, 144));
	//m_entityManager.addEntity("Building", sf::Vector2f(258, 258));
}

bool Game::isRunning() const
{
	return m_window.isOpen();
}

void Game::update()
{
	m_boxSelector.update();
	m_frameTimer.update();
	m_systemManager.update();
	m_window.update();
}

void Game::draw()
{
	m_window.clearWindow();
	m_systemManager.draw(m_window.getWindow());
	DebugOverlay::draw(m_window.getWindow());
	m_boxSelector.draw(m_window.getWindow());
	m_window.display();
}

void Game::lateUpdate()
{
	m_frameTimer.update();
}
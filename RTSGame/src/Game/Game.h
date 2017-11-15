#pragma once

#include <InputEvent.h>
#include <Managers\EventManager.h>
#include <Managers\EntityManager.h>
#include <Managers\SystemManager.h>
#include <Window.h>
#include <Game\BoxSelector.h>
#include <Game\FrameTimer.h>

class Game
{
public:
	Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game&& operator=(Game&&) = delete;

	bool isRunning() const;
	void update();
	void draw();
	void lateUpdate();

private:
	FrameTimer m_frameTimer;
	EventManager<InputEvent> m_inputEventManager;
	Window m_window;
	BoxSelector m_boxSelector;
	EntityManager m_entityManager;
	SystemManager m_systemManager;
};
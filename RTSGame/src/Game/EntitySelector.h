#pragma once

#include <SFML\Graphics.hpp>

template <class InputEvent>
class EventManager;
enum class InputEvent;
class Window;
class SystemManager;
class EntitySelector
{
public:
	EntitySelector(Window& window, EventManager<InputEvent>& eventManager, SystemManager& systemManager);
	~EntitySelector();
	EntitySelector(const EntitySelector&) = delete;
	EntitySelector& operator=(const EntitySelector&) = delete;
	EntitySelector(EntitySelector&&) = delete;
	EntitySelector&& operator = (EntitySelector&&) = delete;

	void update();
	void draw(sf::RenderWindow& window) const;

private:
	Window& m_window;
	EventManager<InputEvent>& m_eventManager;
	SystemManager& m_systemManager;
	bool m_leftMouseButtonHeld;
	bool m_rightMouseButtonDown;
	bool m_secondaryLeftMouseButtonActivated;
	sf::RectangleShape m_rect;
	sf::FloatRect m_rectAABB;

	void onMouseButtonLeftDown();
	void onMouseButtonLeftRelease();
	void onMouseButtonRightDown();
	void onMouseButtonRightRelease();
	void handleEntityCollisions();
};
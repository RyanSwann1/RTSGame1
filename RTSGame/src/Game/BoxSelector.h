#pragma once

#include <SFML\Graphics.hpp>

template <class InputEvent>
class EventManager;
enum class InputEvent;
class Window;
class SystemManager;
class BoxSelector
{
public:
	BoxSelector(Window& window, EventManager<InputEvent>& eventManager, SystemManager& systemManager);
	~BoxSelector();
	BoxSelector(const BoxSelector&) = delete;
	BoxSelector& operator=(const BoxSelector&) = delete;
	BoxSelector(BoxSelector&&) = delete;
	BoxSelector&& operator = (BoxSelector&&) = delete;

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
#pragma once

#include <SFML\Graphics.hpp>

template <class InputEvent>
class EventManager;
enum class InputEvent;
class Window;
class SystemManager;
class BoxSelector
{
	class UnitSelectPositioning
	{
	public:
		UnitSelectPositioning();
		UnitSelectPositioning(const UnitSelectPositioning&) = delete;
		UnitSelectPositioning& operator=(const UnitSelectPositioning&) = delete;
		UnitSelectPositioning(UnitSelectPositioning&&) = delete;
		UnitSelectPositioning&& operator=(UnitSelectPositioning&&) = delete;

		//Debug
		void listAllEntityIDS() const;

		void addEntityID(int entityID);
		void removeEntity(int entityID);
		void clearEntities();
		void assignSelectedUnitsToTargetPositions(const sf::Vector2f& startingPosition);

	private:
		std::vector<int> m_entitiesID;
	};

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
	UnitSelectPositioning m_unitSelectPositioning;
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
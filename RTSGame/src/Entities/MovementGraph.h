#pragma once

#include <SFML\Graphics.hpp>
#include <Game\Point.h>
#include <vector>
#include <memory>

class Entity;
class EntityManager;
enum class Direction;
class SystemManager;
class MovementGraph
{
	friend class Frontier;

	class PositionToMoveTo
	{
	public:
		PositionToMoveTo();

		const sf::Vector2f& getPosition() const;
		void setPosition(const sf::Vector2f& position);
		bool intersecting(const sf::FloatRect& entityAABB) const;

	private:
		sf::Vector2f m_position;
		sf::FloatRect m_AABB;
	};
	
	class Frontier
	{
	public:
		Frontier(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, std::vector<std::unique_ptr<Point>>& graph, int entityID, bool ignoreEntities);
		Frontier(const Frontier&) = delete;
		Frontier& operator=(const Frontier&) = delete;
		Frontier(Frontier&&) = delete;
		Frontier&& operator=(Frontier&&) = delete;

	private:
		const sf::Vector2f& m_targetPosition;
		std::vector<std::unique_ptr<Point>>& m_graph;

		void addToFrontier(const sf::Vector2f& position, std::vector<sf::Vector2f>& frontier, int entityID);
		void addToGraph(const sf::Vector2f& position, int& tileID) const;
		
		sf::Vector2f getNextPoint(const sf::Vector2f& targetPosition, const std::vector<sf::Vector2f>& frontier, int entityID) const;
		bool isOnGraph(const sf::Vector2f& position) const;
	};

public:
	MovementGraph();
	MovementGraph(const MovementGraph&) = delete;
	MovementGraph& operator=(const MovementGraph&) = delete;
	MovementGraph(MovementGraph&&) = delete;
	MovementGraph&& operator=(MovementGraph&&) = delete;

	const sf::Vector2f& getPositionToMoveTo() const;
	void createGraph(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, int entityID);
	void updatePositionToMoveTo(SystemManager& systemManager, EntityManager& entityManager, std::unique_ptr<Entity>& entity);

private:
	std::vector<std::unique_ptr<Point>> m_graph;
	PositionToMoveTo m_positionToMoveTo;

	bool isEntityCollidingWithPositionToMoveTo(EntityManager& entityManager, std::unique_ptr<Entity>& entity);

	void eraseGraphUntilPosition(const sf::Vector2f& position);
	void assignNewPositionToMoveTo(const sf::Vector2f& startingPosition);
	void checkForEntityCollisions(int currentEntityID);
	void changeGraphForEntityCollisions(int currentEntityID);
	void clearGraph();
};
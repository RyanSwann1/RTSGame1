#pragma once

#include <Game\Graph.h>

class Entity;
class EntityManager;
enum class Direction;
class SystemManager;
class ComponentPosition;
class MovementGraph
{
	//--MAIN POINTS--
	//Names Clearer
	//Make control flow easier to read
	//Make header files easier to read - what you see is what you get
	//Make sure class cannot be mis-used - IE calling functions in the wrong order - shouldn't be able to do that

	//Put these classes in the cpp file 
	//Shouldn't hvae to look at it straight away, not used in the header file
	//Aim is simplicity - what you see is what you get
	//What does this class do
	//How much data does this class store
	
	//Dont put it in the header file - doesn't need to be used there
	//Shouldn't be a class
	//Point of a class is to group data together
	//Have a method instead
	//Interface has no public methods - red flag

	//Do not allow class to be mis used
	//Can remove createGraph from the public interface
	//Do not have to class methods in a certain order - if have to == bad design

	class Destination
	{
	public:
		Destination();
		Destination(const Destination&) = delete;
		Destination& operator=(const Destination&) = delete;
		Destination(Destination&&) = delete;
		Destination&& operator=(Destination&&) = delete;

		const sf::FloatRect& getDestination() const;
		sf::Vector2f getPosition() const;
		void setPosition(const sf::Vector2f& newPosition);

	private:
		sf::FloatRect m_destination;
	};
	
	class TargetPosition
	{
	public:
		TargetPosition();
		TargetPosition(const TargetPosition&) = delete;
		TargetPosition& operator=(const TargetPosition&) = delete;
		TargetPosition(TargetPosition&&) = delete;
		TargetPosition&& operator=(TargetPosition&&) = delete;

		bool isTargetPositionReached() const;
		const sf::Vector2f& getTargetPosition() const;
		void reachedTargetPosition();

		void setTargetPosition(const sf::Vector2f& newPosition);
		void reassignTargetToNeighbouringPosition(const sf::Vector2f& position, Graph& graph, int entityID);

	private:
		bool m_reachedTargetPosition;
		sf::Vector2f m_targetPosition;
	};

public:
	MovementGraph();
	MovementGraph(const MovementGraph&) = delete;
	MovementGraph& operator=(const MovementGraph&) = delete;
	MovementGraph(MovementGraph&&) = delete;
	MovementGraph&& operator=(MovementGraph&&) = delete;

	bool entityReachedTargetPosition() const;
	sf::Vector2f getDestination() const;
	
	void createGraph(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, std::unique_ptr<Entity>& entity);
	void updateDestination(SystemManager& systemManager, EntityManager& entityManager, std::unique_ptr<Entity>& entity);
	void onEntityReachingTargetPosition(SystemManager& systemManager, EntityManager& entityManager, std::unique_ptr<Entity>& entity);
	bool isEntityOnTargetPosition(const ComponentPosition& componentPosition) const;

private:
	Graph m_graph;
	Destination m_destination;
	TargetPosition m_targetPosition;

	void assignNewDestination(const sf::Vector2f& startingPosition, std::unique_ptr<Entity>& entity);
	void changeGraphForEntityCollisions(int currentEntityID);
};
#pragma once

#include <Game\Graph.h>

class Entity;
class EntityManager;
enum class Direction;
class SystemManager;
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

public:
	MovementGraph();
	MovementGraph(const MovementGraph&) = delete;
	MovementGraph& operator=(const MovementGraph&) = delete;
	MovementGraph(MovementGraph&&) = delete;
	MovementGraph&& operator=(MovementGraph&&) = delete;

	//Do not allow class to be mis used
	//Can remove createGraph from the public interface
	//Do not have to class methods in a certain order - if have to == bad design

	sf::Vector2f getDestination() const;
	void createGraph(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, std::unique_ptr<Entity>& entity);
	void updatePositionToMoveTo(SystemManager& systemManager, EntityManager& entityManager, std::unique_ptr<Entity>& entity);

private:
	Graph m_graph;
	Destination m_destination;

	void assignNewDestination(const sf::Vector2f& startingPosition, std::unique_ptr<Entity>& entity);
	void changeGraphForEntityCollisions(int currentEntityID);
	void assignDestinationAwayFromEntity(std::unique_ptr<Entity>& entity);
};
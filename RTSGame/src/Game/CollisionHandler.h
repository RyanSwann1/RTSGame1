#pragma once

#include <SFML\Graphics.hpp>
#include <memory>

class Entity;
class EntityManager;
namespace CollisionHandler
{
	bool isEntityAtPosition(const sf::FloatRect& entityAABB, int entityID);
	bool isEntityAtPosition(const sf::Vector2f& position, int entityID);
	bool isEntityCollidingWithDestination(const sf::FloatRect& destination, EntityManager& entityManager, std::unique_ptr<Entity>& entity);
	bool isEntityAtTile(const sf::Vector2f& position, int entityID);
	bool isEntityAtPos(const sf::Vector2f& position);
}
#pragma once

#include <Systems\SystemBase.h>

class EntityManager;
class ComponentAIMovement;
class SystemAIMovement : public SystemBase
{
public:
	SystemAIMovement(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent);
	SystemAIMovement(const SystemAIMovement&) = delete;
	SystemAIMovement& operator=(const SystemAIMovement&) = delete;
	SystemAIMovement(SystemAIMovement&&) = delete;
	SystemAIMovement&& operator=(SystemAIMovement&&) = delete;

	void onSystemDirectMessagePosition(const SystemDirectMessagePosition& systemMessage) const override;
	void update() const override;

private:
	void setNewMovementTargetPosition(const sf::Vector2f& targetPosition, std::unique_ptr<Entity>& entity) const;
	void handleEntityMovement(const EntityManager& entityManager, std::unique_ptr<Entity>& entity) const;

	void updatePositionToMoveTo(EntityManager& entityManager, std::unique_ptr<Entity>& entity) const;
};
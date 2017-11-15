#pragma once

#include <Systems\SystemBase.h>

class SystemPosition : public SystemBase
{
public:
	SystemPosition(SystemManager& systemManager, SystemType type, ComponentType requiredComponent);

	void onSystemDirectMessagePosition(const SystemDirectMessagePosition& systemMessage) const override;
	void update() const override {}

private:
	void setEntityPosition(const sf::Vector2f& position, std::unique_ptr<Entity>& entity) const;
};
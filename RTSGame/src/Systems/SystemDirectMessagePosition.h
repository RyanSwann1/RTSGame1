#pragma once

#include <Systems\SystemDirectMessage.h>
class SystemDirectMessagePosition : public SystemDirectMessage<sf::Vector2f>
{
public:
	SystemDirectMessagePosition(const sf::Vector2f& position, std::unique_ptr<Entity>& entity, SystemEvent systemEvent)
		: SystemDirectMessage(position, entity, systemEvent)
	{}
};
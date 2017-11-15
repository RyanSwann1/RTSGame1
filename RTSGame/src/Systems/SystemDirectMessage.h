#pragma once

#include <memory>
#include <Entities\Entity.h>
#include <Systems\SystemEvent.h>

template <class Message>
class SystemDirectMessage
{
public:
	SystemDirectMessage(const Message& message, std::unique_ptr<Entity>& entity, SystemEvent systemEvent)
		: m_entity(entity),
		m_message(message),
		m_systemEvent(systemEvent)
	{}
		
	std::unique_ptr<Entity>& m_entity;
	const Message m_message;
	const SystemEvent m_systemEvent;
};
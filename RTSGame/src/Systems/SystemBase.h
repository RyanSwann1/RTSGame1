#pragma once

#include <SFML\Graphics.hpp>
#include <Components\ComponentType.h>
#include <Systems\SystemType.h>
#include <memory>

class SystemManager;
enum class SystemEvent;
class SystemDirectMessagePosition;
class Entity;
class SystemMessage;
class SystemBase
{
public:
	SystemBase(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent);
	virtual ~SystemBase() {};
	SystemBase(const SystemBase&) = delete;
	SystemBase& operator=(const SystemBase&) = delete;
	SystemBase(SystemBase&&) = delete;
	SystemBase&& operator=(SystemBase&&) = delete;

	virtual void update() const = 0;
	virtual void onSystemDirectMessagePosition(const SystemDirectMessagePosition& systemMessage) const {}
	virtual void onSystemGlobalEntityMessage(SystemEvent systemEvent) const {}
	void onSystemMessage(const SystemMessage& systemMessage) const;

	const SystemType m_type;

protected:
	const ComponentType m_requiredComponent;
	SystemManager& m_systemManager;

	bool entityFitsRequirements(const std::unique_ptr<Entity>& entity) const;
	bool entityFitsRequirements(const std::unique_ptr<Entity>& entity, ComponentType componentType) const;
	virtual void onSystemEvent(SystemEvent systemEvent, std::unique_ptr<Entity>& entity) const {}
	virtual void onGlobalSystemEvent(SystemEvent systemEvent) const {}
};
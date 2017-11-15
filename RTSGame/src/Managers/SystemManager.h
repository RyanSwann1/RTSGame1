#pragma once

#include <Systems\SystemBase.h>
#include <Systems\SystemMessage.h>
#include <memory>
#include <assert.h>
#include <deque>
#include <array>

class SystemMessageGlobalEntity;
class SystemDirectMessagePosition;
class SystemManager
{
public:
	SystemManager();
	SystemManager(const SystemManager&) = delete;
	SystemManager& operator=(const SystemManager&) = delete;
	SystemManager(SystemManager&&) = delete;
	SystemManager&& operator=(SystemManager&&) = delete;

	void sendSystemDirectMessagePosition(const SystemDirectMessagePosition& systemMessage, SystemType systemType) const;
	void sendGlobalEntitySystemMessage(const SystemMessageGlobalEntity& systemMessageGlobalEntity);
	void addSystemMessage(const SystemMessage& systemMessage);

	void draw(sf::RenderWindow& window) const;
	void update();

private:
	std::array<std::unique_ptr<SystemBase>, static_cast<size_t>(SystemType::Total)> m_systems;
	std::deque<SystemMessage> m_systemMessages;

	template <class System>
	void addSystem(SystemType systemType, ComponentType requiredComponent)
	{
		assert(!m_systems[static_cast<int>(systemType)].get());
		m_systems[static_cast<int>(systemType)] = std::make_unique<System>(*this, systemType, requiredComponent);
	}

	template <class System>
	System& getSystem(SystemType systemType) const
	{
		assert(m_systems[static_cast<int>(systemType)].get());
		auto system = m_systems[static_cast<int>(systemType)].get();
		return *static_cast<System*>(system);
	}

	std::unique_ptr<SystemBase>& getSystemByType(SystemType systemType);
	void handleEvents();
	void sendMessageToSystems(const SystemMessage& systemMessage);
};
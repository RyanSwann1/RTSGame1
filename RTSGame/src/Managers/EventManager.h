#pragma once

#include <unordered_map>
#include <vector>
#include <assert.h>
#include <functional>
#include <utility>

template <class EventType>
class EventManager
{
	class Observer
	{
	public:
		Observer(const std::function<void()>& fp, std::string&& name)
			: m_fp(fp),
			m_name(std::move(name))
		{}

		const std::function<void()> m_fp;
		const std::string m_name;
	};

public:
	EventManager()
		: m_observers()
	{}
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	EventManager(EventManager&&) = delete;
	EventManager& operator=(EventManager&&) = delete;

	void subscribe(EventType eventType, const std::function<void()>& fp, std::string&& name)
	{
		assert(m_observers.find(eventType) == m_observers.cend());
		m_observers.emplace(eventType, std::vector<Observer>{Observer(fp, std::move(name))});
	}

	void notify(EventType eventType)
	{
		auto iter = m_observers.find(eventType);
		assert(iter != m_observers.cend());
		for (auto& observer : iter->second)
		{
			observer.m_fp();
		}
	}

	void unsubscribe(EventType eventType, const std::string& name)
	{
		auto iter = m_observers.find(eventType);
		assert(iter != m_observers.cend());
		//auto observer = std::find_if(iter.second.begin(), iter.second.end(), [&name](const auto& observer) { return observer.m_name == name; });
		//assert(observer != iter.second.end())
		//{
		//	iter.second.erase(observer);
		//}
	}

private:
	std::unordered_map<EventType, std::vector<Observer>> m_observers;
};
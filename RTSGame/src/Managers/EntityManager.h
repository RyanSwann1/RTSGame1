#pragma once

#include <Entities\Entity.h>
#include <list>
#include <functional>
#include <unordered_map>
#include <assert.h>

class EntityToSpawn;
enum class SystemType;
class EntityManager
{
	class EntityInitializer
	{
	public:
		EntityInitializer() {}
		EntityInitializer(const EntityInitializer&) = delete;
		EntityInitializer& operator=(const EntityInitializer) = delete;
		EntityInitializer(EntityInitializer&&) = delete;
		EntityInitializer&& operator=(EntityInitializer&&) = delete;

		void initializeEntityComponents(const sf::Vector2f& startingPosition, std::unique_ptr<Entity>& entity) const;
	};

	class ComponentFactory
	{
	public:
		ComponentFactory();
		ComponentFactory(const ComponentFactory&) = delete;
		ComponentFactory& operator=(const ComponentFactory&) = delete;
		ComponentFactory(ComponentFactory&&) = delete;
		ComponentFactory&& operator=(ComponentFactory&&) = delete;

		std::unique_ptr<ComponentBase> getComponent(ComponentType type) const;

	private:
		std::unordered_map<ComponentType, std::function<std::unique_ptr<ComponentBase>()>> m_factory;

		template <class Component>
		void registerComponent(ComponentType componentType, SystemType systemType)
		{
			assert(m_factory.find(componentType) == m_factory.cend());
			m_factory.emplace(componentType, [componentType, systemType]() -> std::unique_ptr<ComponentBase>
			{
				return std::make_unique<Component>(componentType, systemType);
			});
		}
	};

	class EntityFactory
	{
	public:
		EntityFactory();
		EntityFactory(const EntityFactory&) = delete;
		EntityFactory& operator=(const EntityFactory&) = delete;
		EntityFactory(EntityFactory&&) = delete;
		EntityFactory&& operator=(EntityFactory&&) = delete;
		
		std::unique_ptr<Entity> getEntity(const ComponentFactory& componentFactory, const EntityInitializer& entityInitializer, 
			std::string&& name, const sf::Vector2f& startingPosition, int entityID) const;

	private:
		std::unordered_map<std::string, std::vector<ComponentType>> m_factory;
	
		void registerEntity(std::string&& name, std::vector<ComponentType>&& componentTypes);
	};

public:
	EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;
	EntityManager(EntityManager&&) = delete;
	EntityManager&& operator=(EntityManager&&) = delete;
	
	bool entityHasComponent(const std::unique_ptr<Entity>& entity, ComponentType componentType) const;
	std::list<std::unique_ptr<Entity>>& getEntities();

	template <class Component>
	Component& getEntityComponent(ComponentType componentType, int entityID)
	{
		auto& entity = getEntity(entityID);
		assert(entityHasComponent(entity, componentType));
		return *static_cast<Component*>(entity->m_components[static_cast<unsigned int>(componentType)].get());
	}

	template <class Component>
	Component& getEntityComponent(ComponentType componentType, std::unique_ptr<Entity>& entity) const
	{
		assert(entityHasComponent(entity, componentType));
		return *static_cast<Component*>(entity->m_components[static_cast<unsigned int>(componentType)].get());
	}
	
	void addEntity(std::string&& name, const sf::Vector2f& startingPosition);

private:
	const EntityInitializer m_entityInitializer;
	const ComponentFactory m_componentFactory;
	const EntityFactory m_entityFactory;
	std::list<std::unique_ptr<Entity>> m_entities;
	int m_entityCount; //Acts as each entities ID

	std::unique_ptr<Entity>& getEntity(int entityID);
};
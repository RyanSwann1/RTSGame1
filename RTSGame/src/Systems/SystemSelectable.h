#pragma once

#include <Systems\SystemBase.h>

class SystemSelectable : public SystemBase
{
public:
	SystemSelectable(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent);
	SystemSelectable(const SystemSelectable&) = delete;
	SystemSelectable& operator=(const SystemSelectable&) = delete;
	SystemSelectable(SystemSelectable&&) = delete;
	SystemSelectable&& operator=(SystemSelectable&&) = delete;

	void draw(sf::RenderWindow& window);
	void update() const override;

private:
	void onSystemEvent(SystemEvent systemEvent, std::unique_ptr<Entity>& entity) const override;
	void onGlobalSystemEvent(SystemEvent systemEvent) const override;

	void handleSelection(std::unique_ptr<Entity>& entity) const;
	void handleSingularEntityDeselection(std::unique_ptr<Entity>& entity) const;
	void handleGlobalEntitySelection() const;
};
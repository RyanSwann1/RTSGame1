#pragma once

#include <Systems\SystemBase.h>

class SystemDrawable : public SystemBase
{
public:
	SystemDrawable(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent);
	SystemDrawable(const SystemDrawable&) = delete;
	SystemDrawable& operator=(const SystemDrawable&) = delete;
	SystemDrawable(SystemDrawable&&) = delete;
	SystemDrawable&& operator=(SystemDrawable&&) = delete;

	void draw(sf::RenderWindow& window) const;
	void update() const override;
};
#pragma once

#include <Components\ComponentBase.h>
#include <Game\Direction.h>
#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <array>

enum class ComponentType;
class Entity
{
public:
	Entity(std::string&& name, int ID);

	const std::string m_name;
	const int m_ID;

	std::array<std::unique_ptr<ComponentBase>, MaxComponents> m_components;
};
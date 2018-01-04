#pragma once

#include <Components\ComponentBase.h>
#include <SFML\Graphics.hpp>
#include <Entities\MovementGraph.h>

class ComponentAIMovement : public ComponentBase
{
public:
	ComponentAIMovement(ComponentType type, SystemType owningSystem)
		: ComponentBase(type, owningSystem)
	{}
	ComponentAIMovement(const ComponentAIMovement&) = delete;
	ComponentAIMovement& operator=(const ComponentAIMovement&) = delete;
	ComponentAIMovement(ComponentAIMovement&&) = delete;
	ComponentAIMovement&& operator=(ComponentAIMovement&&) = delete;

	MovementGraph m_movementGraph;
};
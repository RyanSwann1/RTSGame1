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

	MovementGraph m_movementGraph;

};
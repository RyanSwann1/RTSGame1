#pragma once

enum class ComponentType
{
	Position = 0,
	Movable,
	Drawable,
	Selectable,
	Collidable,
	AIMovement,
	AllComponents,
	Spawner,
	Total
};

const size_t MaxComponents = static_cast<size_t>(ComponentType::Total);
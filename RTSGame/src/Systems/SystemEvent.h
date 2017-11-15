#pragma once

enum class SystemEvent
{
	Selected = 0,
	Deselected,
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	MoveUpRight,
	MoveDownRight,
	MoveDownLeft,
	MoveUpLeft,
	StopMovement,
	SetMovementTargetPosition,
	CorrectPosition
};
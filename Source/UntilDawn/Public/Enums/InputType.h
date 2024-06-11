#pragma once

UENUM(BlueprintType)
enum class EInputType : uint8
{
	None,
	LeftClick_Tab,
	LeftClick_Pressed,
	LeftClick_Hold,
	LeftClick_Released,
	LeftClick_Double,
	RightClick_Pressed,
	RightClick_Released,
	R_Pressed,
	R_Hold,
	MouseSide_Pressed,
	Shift_Pressed,
	Shift_Released,
};
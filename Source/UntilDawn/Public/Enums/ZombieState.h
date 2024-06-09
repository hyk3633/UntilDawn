#pragma once

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	IDLE,
	CHASE,
	ATTACK,
	GRAB,
	BITE,
	BLOCKED,
	WAIT,
	Kicked
};
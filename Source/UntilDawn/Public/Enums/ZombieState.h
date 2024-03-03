#pragma once

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	IDLE,
	PATROL,
	CHASE,
	ATTACK,
	GRAB,
	BITE,
	BLOCKED,
	WAIT
};
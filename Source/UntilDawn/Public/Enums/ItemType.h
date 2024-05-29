#pragma once

UENUM(BlueprintType)
enum class EItemMainType : uint8
{
	MeleeWeapon,
	RangedWeapon,
	RecoveryItem,
	AmmoItem,
	ArmourItem,
	MAX,
};
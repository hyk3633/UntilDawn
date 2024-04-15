#pragma once
#include <sstream>
#include "../Enums/ItemType.h"
#include "ItemInfo.generated.h"

union ItemSubType
{
	EMeleeWeaponType meleeWeaponType;
};

USTRUCT()
struct FItemInfo
{
	GENERATED_BODY()

public:

	FItemInfo() {};

	int itemKey;

	FString itemName;

	EItemMainType itemType;

	FIntPoint itemGridSize;

	void CopyTo(FItemInfo* otherInfo)
	{
		otherInfo->itemKey = itemKey;
		otherInfo->itemName = itemName;
		otherInfo->itemType = itemType;
		otherInfo->itemGridSize = itemGridSize;
	}
};

USTRUCT()
struct FMeleeWeaponInfo : public FItemInfo
{
	GENERATED_BODY()

public:

	FMeleeWeaponInfo() {};

	float attackPower;

	float attackSpeed;

	void CopyTo(FMeleeWeaponInfo* otherInfo)
	{
		FItemInfo::CopyTo(otherInfo);
		otherInfo->attackPower = attackPower;
		otherInfo->attackSpeed = attackSpeed;
	}

};

USTRUCT()
struct FRangedWeaponInfo : public FItemInfo
{
	GENERATED_BODY()

public:

	FRangedWeaponInfo() {};

	float attackPower;

	float fireRate;

	float recoil;

	int magazine;

	float reloadingSpeed;

	void CopyTo(FRangedWeaponInfo* otherInfo)
	{
		FItemInfo::CopyTo(otherInfo);
		otherInfo->attackPower		= attackPower;
		otherInfo->fireRate			= fireRate;
		otherInfo->recoil			= recoil;
		otherInfo->magazine			= magazine;
		otherInfo->reloadingSpeed	= reloadingSpeed;
	}

};

USTRUCT()
struct FRecoveryItemInfo : public FItemInfo
{
	GENERATED_BODY()

public:

	FRecoveryItemInfo() {};

	int recoveryAmount;

	float usingSpeed;

	void CopyTo(FRecoveryItemInfo* otherInfo)
	{
		FItemInfo::CopyTo(otherInfo);
		otherInfo->recoveryAmount	= recoveryAmount;
		otherInfo->usingSpeed		= usingSpeed;
	}

};

USTRUCT()
struct FAmmoItemInfo : public FItemInfo
{
	GENERATED_BODY()

public:

	FAmmoItemInfo() {};

	int amount;

	int ammoType;

	void CopyTo(FAmmoItemInfo* otherInfo)
	{
		FItemInfo::CopyTo(otherInfo);
		otherInfo->amount		= amount;
		otherInfo->ammoType		= ammoType;
	}

};
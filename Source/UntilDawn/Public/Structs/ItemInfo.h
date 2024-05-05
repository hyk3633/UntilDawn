#pragma once
#include <sstream>
#include "../Enums/ItemType.h"
#include "ItemInfo.generated.h"

USTRUCT()
struct FItemInfo
{
	GENERATED_BODY()

public:

	FItemInfo() {}

	int itemKey;

	FString itemName;

	EItemMainType itemType;

	FIntPoint itemGridSize;

	bool isConsumable;

	EItemMainType GetItemType() const { return StaticCast<EItemMainType>(itemType); }
};

USTRUCT()
struct FWeaponInfo
{
	GENERATED_BODY()

public:

	FWeaponInfo() {}

	int weaponType;

	float attackPower;

	void CopyTo(FWeaponInfo* otherInfo)
	{
		otherInfo->weaponType = weaponType;
		otherInfo->attackPower = attackPower;
	}

};

/*USTRUCT()
struct FMeleeWeaponInfo : public FWeaponInfo
{
	GENERATED_BODY()

public:

	FMeleeWeaponInfo() {}

	float attackPower;

	float attackSpeed;

	void CopyTo(FMeleeWeaponInfo* otherInfo)
	{
		FItemInfo::CopyTo(otherInfo);
		otherInfo->attackPower = attackPower;
		otherInfo->attackSpeed = attackSpeed;
	}

};*/

USTRUCT()
struct FRangedWeaponInfo : public FWeaponInfo
{
	GENERATED_BODY()

public:

	FRangedWeaponInfo() {}

	float fireRate;

	float recoil;

	int magazine;

	float reloadingSpeed;

	void CopyTo(FRangedWeaponInfo* otherInfo)
	{
		FWeaponInfo::CopyTo(otherInfo);
		otherInfo->fireRate			= fireRate;
		otherInfo->recoil			= recoil;
		otherInfo->magazine			= magazine;
		otherInfo->reloadingSpeed	= reloadingSpeed;
	}

	friend FArchive& operator<<(FArchive& archive, FRangedWeaponInfo& info)
	{
		archive << info.attackPower;
		archive << info.weaponType;
		archive << info.fireRate;
		archive << info.recoil;
		archive << info.magazine;
		archive << info.reloadingSpeed;
		return archive;
	}

};

USTRUCT()
struct FRecoveryItemInfo
{
	GENERATED_BODY()

public:

	FRecoveryItemInfo() {}

	int recoveryAmount;

	float usingSpeed;

	void CopyTo(FRecoveryItemInfo* otherInfo)
	{
		otherInfo->recoveryAmount	= recoveryAmount;
		otherInfo->usingSpeed		= usingSpeed;
	}

};

USTRUCT()
struct FAmmoItemInfo
{
	GENERATED_BODY()

public:

	FAmmoItemInfo() {}

	int amount;

	int ammoType;

	void CopyTo(FAmmoItemInfo* otherInfo)
	{
		otherInfo->amount		= amount;
		otherInfo->ammoType		= ammoType;
	}

};
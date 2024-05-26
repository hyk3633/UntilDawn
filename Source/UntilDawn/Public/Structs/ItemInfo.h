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
	uint16 quantity;
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

	float attackPower;

	void Parsing(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
	{
		attackPower = concreteInfoMap["AttackPower"]->AsNumber();
	}
};

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

	void Parsing(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
	{
		FWeaponInfo::Parsing(concreteInfoMap);
		fireRate		= concreteInfoMap["FireRate"]->AsNumber();
		recoil			= concreteInfoMap["Recoil"]->AsNumber();
		magazine		= concreteInfoMap["Magazine"]->AsNumber();
		reloadingSpeed	= concreteInfoMap["ReloadingSpeed"]->AsNumber();
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

	void Parsing(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
	{
		recoveryAmount	= concreteInfoMap["RecoveryAmount"]->AsNumber();
		usingSpeed		= concreteInfoMap["UsingSpeed"]->AsNumber();
	}

};

USTRUCT()
struct FAmmoItemInfo
{
	GENERATED_BODY()

public:

	FAmmoItemInfo() {}

	int ammoType;

	void Parsing(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
	{
		ammoType = concreteInfoMap["AmmoType"]->AsNumber();
	}

};
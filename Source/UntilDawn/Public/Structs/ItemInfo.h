#pragma once
#include <sstream>
#include "../Enums/ItemType.h"
#include "../Enums/WeaponType.h"
#include "../Enums/ArmourSlot.h"
#include "../Enums/AmmoType.h"
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
	EWeaponType weaponType;

	void Parsing(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
	{
		attackPower = concreteInfoMap["AttackPower"]->AsNumber();
		weaponType = StaticCast<EWeaponType>(concreteInfoMap["WeaponType"]->AsNumber());
	}
};

USTRUCT()
struct FRangedWeaponInfo : public FWeaponInfo
{
	GENERATED_BODY()

public:

	FRangedWeaponInfo() {}

	int magazine;

	EAmmoType ammoType;

	void Parsing(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
	{
		FWeaponInfo::Parsing(concreteInfoMap);
		magazine		= concreteInfoMap["Magazine"]->AsNumber();
		ammoType		= StaticCast<EAmmoType>(concreteInfoMap["AmmoType"]->AsNumber());
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

	EAmmoType ammoType;

	void Parsing(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
	{
		ammoType = StaticCast<EAmmoType>(concreteInfoMap["AmmoType"]->AsNumber());
	}

};

USTRUCT()
struct FArmourItemInfo
{
	GENERATED_BODY()

public:

	FArmourItemInfo() {}

	float defensePower;

	EArmourSlot armourSlot;

	void Parsing(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
	{
		defensePower = concreteInfoMap["DefensePower"]->AsNumber();
		armourSlot = StaticCast<EArmourSlot>(concreteInfoMap["ArmourSlot"]->AsNumber());
	}

};
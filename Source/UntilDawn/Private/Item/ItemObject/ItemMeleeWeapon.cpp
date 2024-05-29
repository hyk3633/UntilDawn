// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemMeleeWeapon.h"
#include "Item/ItemFunction/FunctionMeleeAttack.h"

void UItemMeleeWeapon::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemMeleeWeapon::MakeItemFunction()
{
	meleeAttackFunction = MakeShared<FunctionMeleeAttack>();
}

uint8 UItemMeleeWeapon::GetItemSubType() const
{
	return StaticCast<uint8>(concreteInfo.weaponType);
}

void UItemMeleeWeapon::Using(USkeletalMeshComponent* itemMesh)
{
	meleeAttackFunction->MeleeAttack(GetOwnerController(), itemMesh, concreteInfo.attackPower);
}
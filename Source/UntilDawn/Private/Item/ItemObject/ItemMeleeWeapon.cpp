// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemMeleeWeapon.h"
#include "Item/ItemFunction/FunctionMeleeAttack.h"

void UItemMeleeWeapon::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	Super::ParseItemConcreteInfo(concreteInfoMap);
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemMeleeWeapon::MakeItemFunction()
{
	meleeAttackFunction = MakeShared<FunctionMeleeAttack>();
}

void UItemMeleeWeapon::Using(USkeletalMeshComponent* itemMesh)
{
	meleeAttackFunction->MeleeAttack(GetOwnerController(), itemMesh, concreteInfo.attackPower);
}
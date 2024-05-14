// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemProjectileWeapon.h"
#include "Item/ItemFunction/FunctionProjectileShooting.h"
#include "UntilDawn/UntilDawn.h"

void UItemProjectileWeapon::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemProjectileWeapon::MakeItemFunction()
{
	shootingFunction = MakeShared<FunctionProjectileShooting>();
}

void UItemProjectileWeapon::Using(TWeakObjectPtr<APlayerController> playerController, USkeletalMeshComponent* itemMesh)
{
	// ź���� �ִ��� �˻�
	shootingFunction->Shooting(playerController, itemMesh);
}

EWeaponType UItemProjectileWeapon::GetWeaponType()
{
	return StaticCast<EWeaponType>(concreteInfo.weaponType);
}

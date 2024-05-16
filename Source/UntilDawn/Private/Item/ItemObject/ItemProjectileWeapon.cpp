// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemProjectileWeapon.h"
#include "Item/ItemObject/ItemAmmo.h"
#include "Item/ItemFunction/FunctionProjectileShooting.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "UntilDawn/UntilDawn.h"

void UItemProjectileWeapon::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	Super::ParseItemConcreteInfo(concreteInfoMap);
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemProjectileWeapon::MakeItemFunction()
{
	shootingFunction = MakeShared<FunctionProjectileShooting>();
}

void UItemProjectileWeapon::Reload(TWeakObjectPtr<APlayerController> playerController)
{
	TWeakObjectPtr<APlayerControllerMainMap> playerControllerMainMap = Cast<APlayerControllerMainMap>(playerController);
	auto itemObj = playerControllerMainMap->GetItemObjectOfType(EItemMainType::AmmoItem);
	TWeakObjectPtr<UItemAmmo> ammoItemObj = Cast<UItemAmmo>(itemObj);
	check(ammoItemObj.IsValid());

	const uint16 remainedAmmo = ammoItemObj->Using(concreteInfo.magazine - loadedAmmoAmount);
	if (remainedAmmo)
	{
		// ������ �ִϸ��̼�
		loadedAmmoAmount += remainedAmmo;
	}
}

void UItemProjectileWeapon::Using(TWeakObjectPtr<APlayerController> playerController, USkeletalMeshComponent* itemMesh)
{
	// ź���� ������ ������ -> ź�� �������� ���� �� ź�� �� ��ŭ ����ؼ� ź�� ������ ���� ����
	if (playerController.IsValid())
	{
		if (loadedAmmoAmount == 0)
		{
			Reload(playerController);
		}
		if (loadedAmmoAmount)
		{
			shootingFunction->Shooting(playerController, itemMesh);
			loadedAmmoAmount--;
		}
	}
}
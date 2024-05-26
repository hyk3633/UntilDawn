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

void UItemProjectileWeapon::Reload()
{
	auto itemObj = GetOwnerController()->GetItemObjectOfType(EItemMainType::AmmoItem);
	TWeakObjectPtr<UItemAmmo> ammoItemObj = Cast<UItemAmmo>(itemObj);
	if (ammoItemObj.IsValid() == false)
		return;

	uint16 neededAmmo = StaticCast<uint16>(concreteInfo.magazine) - loadedAmmoAmount;
	if (neededAmmo)
	{
		neededAmmo = FMath::Min(neededAmmo, ammoItemObj->GetItemQuantity());
		loadedAmmoAmount += neededAmmo;
		ammoItemObj->Using(neededAmmo);

		onFireWeapon.ExecuteIfBound(loadedAmmoAmount);
		
		// 재장전 애니메이션
	}
}

void UItemProjectileWeapon::Using(USkeletalMeshComponent* itemMesh)
{
	if (GetOwnerController().IsValid())
	{
		if (loadedAmmoAmount == 0)
		{
			Reload();
		}
		if (loadedAmmoAmount)
		{
			shootingFunction->Shooting(GetOwnerController(), itemMesh);
			loadedAmmoAmount--;
			onFireWeapon.ExecuteIfBound(loadedAmmoAmount);
		}
	}
}

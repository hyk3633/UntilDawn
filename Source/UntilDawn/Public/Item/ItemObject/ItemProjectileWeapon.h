// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject/ItemPermanent.h"
#include "../../Structs/ItemInfo.h"
#include "ItemProjectileWeapon.generated.h"

/**
 * 
 */

class FunctionProjectileShooting;

UCLASS()
class UNTILDAWN_API UItemProjectileWeapon : public UItemPermanent
{
	GENERATED_BODY()

public:

	UItemProjectileWeapon() = default;
	~UItemProjectileWeapon() = default;

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

public:

	void Reload(TWeakObjectPtr<APlayerController> playerController);

	virtual void Using(TWeakObjectPtr<APlayerController> playerController, USkeletalMeshComponent* itemMesh = nullptr) override;

private:

	TSharedPtr<FunctionProjectileShooting> shootingFunction;

	FRangedWeaponInfo concreteInfo;

	uint16 loadedAmmoAmount = 0;
	
};

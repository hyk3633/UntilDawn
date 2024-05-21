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

	void Reload();

	virtual void Using(USkeletalMeshComponent* itemMesh = nullptr) override;

	bool HasAmmo();

private:

	TSharedPtr<FunctionProjectileShooting> shootingFunction;

	FRangedWeaponInfo concreteInfo;

	uint8 loadedAmmoAmount = 0;
	
};

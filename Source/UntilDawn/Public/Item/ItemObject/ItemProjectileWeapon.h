// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject/ItemPermanent.h"
#include "../../Structs/ItemInfo.h"
#include "ItemProjectileWeapon.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(OnFireWeapon, uint16 loadedAmmo);

class FunctionProjectileShooting;

UCLASS()
class UNTILDAWN_API UItemProjectileWeapon : public UItemPermanent
{
	GENERATED_BODY()

public:

	UItemProjectileWeapon() = default;
	~UItemProjectileWeapon() = default;

	OnFireWeapon onFireWeapon;

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

public:

	void Reload();

	virtual void Using(USkeletalMeshComponent* itemMesh = nullptr) override;

	uint16 GetMagazineAmount() const { return concreteInfo.magazine; }

	uint16 GetLoadedAmmoAmount() const { return loadedAmmoAmount; }

private:

	TSharedPtr<FunctionProjectileShooting> shootingFunction;

	FRangedWeaponInfo concreteInfo;

	uint16 loadedAmmoAmount = 0;
	
};

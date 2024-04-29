// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/ItemWeapon.h"
#include "../../Structs/ItemInfo.h"
#include "../../Enums/WeaponType.h"
#include "ItemRangedWeapon.generated.h"

/**
 * 
 */

class UActorPooler;
class UShootingComponent;

UCLASS()
class UNTILDAWN_API AItemRangedWeapon : public AItemWeapon
{
	GENERATED_BODY()

public:

	AItemRangedWeapon();

	void InitializeRangedWeaponInfo(const FRangedWeaponInfo& newInfo);

	FORCEINLINE FRangedWeaponInfo GetRangedInfo() const { return rangedWeaponInfo; }

	virtual void Attack(TWeakObjectPtr<APlayerController> ownerController) override;

	virtual EWeaponType GetWeaponType() const override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	UShootingComponent* shootingComponent;
	
	UPROPERTY()
	UActorPooler* projectilePooler;

	FRangedWeaponInfo rangedWeaponInfo;

};

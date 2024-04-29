// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/ItemWeapon.h"
#include "../../Structs/ItemInfo.h"
#include "../../Enums/WeaponType.h"
#include "ItemMeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API AItemMeleeWeapon : public AItemWeapon
{
	GENERATED_BODY()
	
public:

	AItemMeleeWeapon();

	void InitializeWeaponInfo(const FWeaponInfo& newInfo);

	FORCEINLINE FWeaponInfo GetWeaponInfo() const { return weaponInfo; }

	void ActivateAttackTrace(FHitResult& hit);

	virtual void Attack(TWeakObjectPtr<APlayerController> ownerController) override;

	virtual EWeaponType GetWeaponType() const override;

protected:



private:

	FWeaponInfo weaponInfo;

};

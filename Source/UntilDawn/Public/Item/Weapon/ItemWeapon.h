// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "../../Enums/WeaponType.h"
#include "ItemWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API AItemWeapon : public AItemBase
{
	GENERATED_BODY()

public:

	virtual void Attack(TWeakObjectPtr<APlayerController> ownerController);
	
	virtual EWeaponType GetWeaponType() const;

};

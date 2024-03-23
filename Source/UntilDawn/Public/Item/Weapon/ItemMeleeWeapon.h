// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "ItemMeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API AItemMeleeWeapon : public AItemBase
{
	GENERATED_BODY()
	
public:

	AItemMeleeWeapon();

	void ActivateAttackTrace(FHitResult& hit);

protected:



private:



};

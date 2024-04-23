// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "ItemRangedWeapon.generated.h"

/**
 * 
 */

class UActorPooler;

UCLASS()
class UNTILDAWN_API AItemRangedWeapon : public AItemBase
{
	GENERATED_BODY()

public:

	AItemRangedWeapon();

protected:

	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	UActorPooler* projectilePooler;

};

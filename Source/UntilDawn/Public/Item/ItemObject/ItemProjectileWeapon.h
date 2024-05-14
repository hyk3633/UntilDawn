// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Enums/WeaponType.h"
#include "Item/ItemObject.h"
#include "../../Structs/ItemInfo.h"
#include "ItemProjectileWeapon.generated.h"

/**
 * 
 */

class FunctionProjectileShooting;

UCLASS()
class UNTILDAWN_API UItemProjectileWeapon : public UItemObject
{
	GENERATED_BODY()

public:

	UItemProjectileWeapon() = default;
	~UItemProjectileWeapon() = default;

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

public:

	virtual void Using(TWeakObjectPtr<APlayerController> playerController, USkeletalMeshComponent* itemMesh = nullptr) override;

	EWeaponType GetWeaponType();

private:

	TSharedPtr<FunctionProjectileShooting> shootingFunction;

	FRangedWeaponInfo concreteInfo;
	
};

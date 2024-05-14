// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject.h"
#include "../../Structs/ItemInfo.h"
#include "../../Enums/WeaponType.h"
#include "ItemMeleeWeapon.generated.h"

/**
 * 
 */

class FunctionMeleeAttack;

UCLASS()
class UNTILDAWN_API UItemMeleeWeapon : public UItemObject
{
	GENERATED_BODY()

public:

	UItemMeleeWeapon() = default;
	~UItemMeleeWeapon() = default;

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

public:

	virtual void Using(TWeakObjectPtr<APlayerController> playerController, USkeletalMeshComponent* itemMesh = nullptr) override;

	EWeaponType GetWeaponType();

private:

	TSharedPtr<FunctionMeleeAttack> meleeAttackFunction;

	FWeaponInfo concreteInfo;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject/ItemPermanent.h"
#include "../../Structs/ItemInfo.h"
#include "ItemMeleeWeapon.generated.h"

/**
 * 
 */

class FunctionMeleeAttack;

UCLASS()
class UNTILDAWN_API UItemMeleeWeapon : public UItemPermanent
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

private:

	TSharedPtr<FunctionMeleeAttack> meleeAttackFunction;

	FWeaponInfo concreteInfo;
	
};

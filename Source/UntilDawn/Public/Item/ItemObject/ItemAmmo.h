// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject/ItemConsumable.h"
#include "../../Structs/ItemInfo.h"
#include "../../Enums/AmmoType.h"
#include "ItemAmmo.generated.h"

/**
 * 
 */

class APlayerCharacter;

UCLASS()
class UNTILDAWN_API UItemAmmo : public UItemConsumable
{
	GENERATED_BODY()

public:

	virtual void Using(const uint16 neededAmount = 1) override;

	EAmmoType GetAmmoType() const;

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

private:

	FAmmoItemInfo concreteInfo;
	
};

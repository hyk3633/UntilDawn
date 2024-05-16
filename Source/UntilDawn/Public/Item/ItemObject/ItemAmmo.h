// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject/ItemConsumable.h"
#include "../../Structs/ItemInfo.h"
#include "ItemAmmo.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UItemAmmo : public UItemConsumable
{
	GENERATED_BODY()

public:

	virtual uint16 Using(const uint16 neededAmount) override;

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

private:

	FAmmoItemInfo concreteInfo;
	
};

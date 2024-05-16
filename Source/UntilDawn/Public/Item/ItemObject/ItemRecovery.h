// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject/ItemConsumable.h"
#include "../../Structs/ItemInfo.h"
#include "ItemRecovery.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UItemRecovery : public UItemConsumable
{
	GENERATED_BODY()
	
public:

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

public:

	virtual uint16 Using(const uint16 neededAmount) override;

private:

	FRecoveryItemInfo concreteInfo;

};

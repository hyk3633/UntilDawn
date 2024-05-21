// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject/ItemConsumable.h"
#include "../../Structs/ItemInfo.h"
#include "ItemRecovery.generated.h"

/**
 * 
 */

class APlayerCharacter;

UCLASS()
class UNTILDAWN_API UItemRecovery : public UItemConsumable
{
	GENERATED_BODY()
	
public:

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

public:

	virtual void Using(const uint16 neededAmount = 1) override;

private:

	FRecoveryItemInfo concreteInfo;

};

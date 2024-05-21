// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject.h"
#include "ItemConsumable.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(DelegateItemExhaust, FString itemID);

class APlayerControllerMainMap;
class APlayerCharacter;

UCLASS(Abstract)
class UNTILDAWN_API UItemConsumable : public UItemObject
{
	GENERATED_BODY()
	
protected:

	void Consumed(const uint16 consumedAmount);

public:

	virtual void Using(const uint16 neededAmount = 1) PURE_VIRTUAL(UItemConsumable::Using);
	
	DelegateItemExhaust DItemExhaust;

};

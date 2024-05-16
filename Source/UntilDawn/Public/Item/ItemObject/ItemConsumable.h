// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject.h"
#include "ItemConsumable.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNTILDAWN_API UItemConsumable : public UItemObject
{
	GENERATED_BODY()

protected:

	void Consumed(const uint16 consumedAmount);

public:

	virtual uint16 Using(const uint16 neededAmount) PURE_VIRTUAL(UItemConsumable::Using, return 0;);
	
};

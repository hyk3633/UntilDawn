// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject.h"
#include "ItemPermanent.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UItemPermanent : public UItemObject
{
	GENERATED_BODY()

public:

	virtual void Using(USkeletalMeshComponent* itemMesh = nullptr) PURE_VIRTUAL(UItemPermanant::Using, );
	
	virtual uint8 GetItemSubType() const PURE_VIRTUAL(UItemPermanant::GetItemSubType, return 0;);

	void SetEquippedSlotNumber(const int8 number);

	int8 GetEquippedSlotNumber() const;

private:

	int8 equippedSlotNumber = -1;

};

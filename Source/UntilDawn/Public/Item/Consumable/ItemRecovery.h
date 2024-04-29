// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "../../Structs/ItemInfo.h"
#include "ItemRecovery.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API AItemRecovery : public AItemBase
{
	GENERATED_BODY()
	
public:

	void InitializeRecoveryInfo(const FRecoveryItemInfo& newInfo);

	FORCEINLINE FRecoveryItemInfo GetRecoveryInfo() const { return recoveryItemInfo; }

protected:


private:

	FRecoveryItemInfo recoveryItemInfo;

};

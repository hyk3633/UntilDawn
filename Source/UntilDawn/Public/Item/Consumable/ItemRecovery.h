// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "../../Structs/ItemInfo.h"
#include "../../Interface/UsingItem.h"
#include "ItemRecovery.generated.h"

/**
 * 
 */

class UInstantRecoveryComponent;

UCLASS()
class UNTILDAWN_API AItemRecovery : public AItemBase, public IUsingItem
{
	GENERATED_BODY()
	
public:

	AItemRecovery();

	void InitializeRecoveryInfo(const FRecoveryItemInfo& newInfo);

	FORCEINLINE FRecoveryItemInfo GetRecoveryInfo() const { return recoveryItemInfo; }

	virtual void UsingItem() override;

protected:

private:

	UPROPERTY()
	UInstantRecoveryComponent* instantRecoveryComponent;

	FRecoveryItemInfo recoveryItemInfo;

};

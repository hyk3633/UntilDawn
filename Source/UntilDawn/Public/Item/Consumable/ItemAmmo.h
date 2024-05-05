// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "../../Structs/ItemInfo.h"
#include "../../Interface/UsingItem.h"
#include "ItemAmmo.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API AItemAmmo : public AItemBase, public IUsingItem
{
	GENERATED_BODY()
	
public:

	void InitializeAmmoInfo(const FAmmoItemInfo& newInfo);

	FORCEINLINE FAmmoItemInfo GetAmmoInfo() const { return ammoItemInfo; }

	virtual void UsingItem() override;

protected:


private:

	FAmmoItemInfo ammoItemInfo;

};

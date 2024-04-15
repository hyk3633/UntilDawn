// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Structs/ItemAsset.h"
#include "../Structs/ItemInfo.h"
#include "CoreMinimal.h"

/**
 * 
 */
class UNTILDAWN_API ItemCore
{
public:

	ItemCore();

	ItemCore(FItemInfo* newInfo, TSharedPtr<FItemAsset> newAsset);

	~ItemCore();
	
	FORCEINLINE TSharedPtr<FItemAsset> GetItemAsset() const { return itemAsset; };

private:

	TSharedPtr<FItemAsset> itemAsset;

	FItemInfo* itemInfo;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Structs/ItemAsset.h"
#include "../Structs/ItemInfo.h"
#include "ItemObject.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(DelegateItemPicked, int itemID);

UCLASS()
class UNTILDAWN_API UItemObject : public UObject
{
	GENERATED_BODY()

public:

	UItemObject();

	void Init(const int id, FItemInfo* newInfo, TSharedPtr<FItemAsset> newAsset);

	~UItemObject();

	FORCEINLINE TSharedPtr<FItemAsset> GetItemAsset() const { return itemAsset; };

	FIntPoint GetDimensions() const;

	UMaterialInstance* GetIcon() const;

	void Rotate();

	FORCEINLINE bool IsRotated() const { return rotated; }

	FORCEINLINE int GetItemID() const { return itemID; }

	FORCEINLINE int GetItemType() const { return static_cast<int>(itemInfo->itemType); }

	DelegateItemPicked DItemPicked;

private:

	int itemID;

	TSharedPtr<FItemAsset> itemAsset;

	FItemInfo* itemInfo;

	bool rotated;
	
};

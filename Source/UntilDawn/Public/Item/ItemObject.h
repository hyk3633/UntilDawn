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

	void Init(const int id, FItemInfo* newInfo, FItemAsset* newAsset);

	virtual void BeginDestroy() override;

	USkeletalMesh* GetSkeletalMesh() const { return itemAsset.skeletalMesh; }

	UStaticMesh* GetStaticMesh() const { return itemAsset.staticMesh; }

	FIntPoint GetDimensions() const;

	FIntPoint GetRotatedDimensions() const;

	UMaterialInstance* GetIcon() const;

	UMaterialInstance* GetRotatedIcon() const;

	void Rotate();

	FORCEINLINE bool IsRotated() const { return rotated; }

	FORCEINLINE int GetItemID() const { return itemID; }

	FORCEINLINE int GetItemType() const { return static_cast<int>(itemInfo->itemType); }

	void SetTopLeftIndex(const int index);

	FORCEINLINE int GetTopLeftIndex() const { return topLeftIndex; }

	DelegateItemPicked DItemPicked;

private:

	int itemID;

	FItemAsset itemAsset;

	FItemInfo* itemInfo;

	bool rotated;

	int topLeftIndex;
	
};

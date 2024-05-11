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

UCLASS()
class UNTILDAWN_API UItemObject : public UObject
{
	GENERATED_BODY()

public:

	UItemObject();

	void Init(const FString& itemId, FItemInfo newInfo, FItemAsset newAsset);

	virtual void BeginDestroy() override;

	USkeletalMesh* GetSkeletalMesh() const { return itemAsset.skeletalMesh; }

	UStaticMesh* GetStaticMesh() const { return itemAsset.staticMesh; }

	FName GetSocketName() const { return itemAsset.socketName; }

	FIntPoint GetDimensions() const;

	FIntPoint GetRotatedDimensions() const;

	UMaterialInstance* GetIcon() const;

	UMaterialInstance* GetRotatedIcon() const;

	void Rotate();

	FORCEINLINE bool IsRotated() const { return rotated; }

	FORCEINLINE FString GetItemID() const { return itemID; }

	FORCEINLINE int GetItemType() const { return static_cast<int>(itemInfo.itemType); }

	FORCEINLINE bool GetIsConsumable() const { return itemInfo.isConsumable; }

	void SetTopLeftIndex(const int index);

	FORCEINLINE int GetTopLeftIndex() const { return topLeftIndex; }

	FORCEINLINE FItemInfo GetItemInfo() const { return itemInfo; }

	void SetItemQuantity(const uint8 quantity);

private:

	FString itemID;

	FItemAsset itemAsset;

	FItemInfo itemInfo;

	bool rotated;

	int topLeftIndex;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Structs/ItemAsset.h"
#include "../Structs/ItemInfo.h"
#include "../Structs/Tile.h"
#include "Serialization/BufferArchive.h"
#include "ItemObject.generated.h"

/**
 * 
 */

class AItemBase;

UCLASS(Abstract)
class UNTILDAWN_API UItemObject : public UObject
{
	GENERATED_BODY()

public:

	UItemObject();
	~UItemObject() = default;

	void Init(const FString& itemId, FItemInfo newInfo, TMap<FString, TSharedPtr<FJsonValue>> concreteInfoMap, FItemAsset newAsset);

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

	void SetTopLeft(const FTile& newTopLeft);

	FORCEINLINE FTile GetTopLeft() const { return topLeft; }

	FORCEINLINE FItemInfo GetItemInfo() const { return itemInfo; }

	void SetItemQuantity(const uint8 quantity);

	FORCEINLINE uint16 GetItemQuantity() const { return itemInfo.quantity; }

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) PURE_VIRTUAL(UItemObject::ParseItemConcreteInfo, );

	virtual void MakeItemFunction() PURE_VIRTUAL(UItemObject::MakeItemFunction, );

private:

	FString itemID;

	FItemAsset itemAsset;

	FItemInfo itemInfo;

	bool rotated;

	FTile topLeft;
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"

UItemObject::UItemObject()
{
	
}

void UItemObject::Init(const FString& itemId, FItemInfo newInfo, TMap<FString, TSharedPtr<FJsonValue>> concreteInfoMap, FItemAsset newAsset)
{
	itemID = itemId;
	itemInfo = newInfo;
	itemAsset = newAsset;
	ParseItemConcreteInfo(concreteInfoMap);
	MakeItemFunction();
}

void UItemObject::BeginDestroy()
{
	Super::BeginDestroy();
	
}

FIntPoint UItemObject::GetDimensions() const
{
	if (IsRotated())
	{
		return FIntPoint(itemInfo.itemGridSize.Y, itemInfo.itemGridSize.X);
	}
	else
	{
		return itemInfo.itemGridSize;
	}
}

FIntPoint UItemObject::GetRotatedDimensions() const
{
	return FIntPoint(itemInfo.itemGridSize.Y, itemInfo.itemGridSize.X);
}

UMaterialInstance* UItemObject::GetIcon() const
{
	if (rotated)
	{
		return itemAsset.iconRotated;
	}
	else
	{
		return itemAsset.icon;
	}
}

UMaterialInstance* UItemObject::GetRotatedIcon() const
{
	return itemAsset.iconRotated;
}

void UItemObject::Rotate()
{
	rotated = !rotated;
}

void UItemObject::SetTopLeftIndex(const int index)
{
	topLeftIndex = index;
}

void UItemObject::SetItemQuantity(const uint8 quantity)
{
	itemInfo.quantity = quantity;
}

void UItemObject::Using(TWeakObjectPtr<APlayerController> playerController, USkeletalMeshComponent* itemMesh)
{
}

void UItemObject::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
}

void UItemObject::MakeItemFunction()
{
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"

UItemObject::UItemObject()
{
}

void UItemObject::Init(const int id, FItemInfo* newInfo, FItemAsset* newAsset)
{
	itemID = id;
	itemInfo = new FItemInfo();
	newInfo->CopyTo(itemInfo);
	itemAsset.Copy(newAsset);
}

void UItemObject::BeginDestroy()
{
	Super::BeginDestroy();

	if (itemInfo)
	{
		delete itemInfo;
	}
}

FIntPoint UItemObject::GetDimensions() const
{
	if (IsRotated())
	{
		return FIntPoint(itemInfo->itemGridSize.Y, itemInfo->itemGridSize.X);
	}
	else
	{
		return itemInfo->itemGridSize;
	}
}

FIntPoint UItemObject::GetRotatedDimensions() const
{
	return FIntPoint(itemInfo->itemGridSize.Y, itemInfo->itemGridSize.X);
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"

UItemObject::UItemObject()
{
}

void UItemObject::Init(const int id, FItemInfo* newInfo, TSharedPtr<FItemAsset> newAsset)
{
	itemID = id;
	itemInfo = new FItemInfo();
	newInfo->CopyTo(itemInfo);
	itemAsset = newAsset;
}

UItemObject::~UItemObject()
{
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

UMaterialInstance* UItemObject::GetIcon() const
{
	if (rotated)
	{
		return itemAsset->iconRotated;
	}
	else
	{
		return itemAsset->icon;
	}
}

void UItemObject::Rotate()
{
	rotated = !rotated;
}

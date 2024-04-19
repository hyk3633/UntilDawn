// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemCore.h"

ItemCore::ItemCore()
{
}

ItemCore::ItemCore(FItemInfo* newInfo, TSharedPtr<FItemAsset> newAsset)
{
	itemInfo = new FItemInfo();
	newInfo->CopyTo(itemInfo);
	itemAsset = newAsset;
}

ItemCore::~ItemCore()
{
	delete itemInfo;
}

FIntPoint ItemCore::GetDimensions() const
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

UMaterialInstance* ItemCore::GetIcon() const
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

void ItemCore::Rotate()
{
	rotated = !rotated;
}
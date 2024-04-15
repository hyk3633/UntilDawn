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

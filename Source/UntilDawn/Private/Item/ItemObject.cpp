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

void UItemObject::SetTopLeft(const FTile& newTopLeft)
{
	topLeft = newTopLeft;
}

void UItemObject::SetItemQuantity(const uint8 quantity)
{
	itemInfo.quantity = quantity;
	DUpdateItemQuantity.ExecuteIfBound(quantity);
}

void UItemObject::SetOwnerController(TWeakObjectPtr<APlayerControllerMainMap> controller)
{
	ownerController = controller;
}

void UItemObject::SetOwnerCharacter(TWeakObjectPtr<APlayerCharacter> character)
{
	ownerCharacter = character;
}

void UItemObject::ResetOwner()
{
	ownerController.Reset();
	ownerCharacter.Reset();
}

TSubclassOf<UGameplayAbility> UItemObject::GetAbility(const EInputType inputType)
{
	if (itemAsset.abilities.Find(inputType))
	{
		return itemAsset.abilities[inputType];
	}
	return nullptr;
}

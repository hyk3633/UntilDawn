// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/InventoryComponent.h"
#include "Player/PlayerCharacter.h"
#include "Item/ItemObject.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject/ItemPermanent.h"
#include "Item/ItemObject/ItemConsumable.h"
#include "Item/ItemObject/ItemProjectileWeapon.h"
#include "GameMode/GameModeMainMap.h"
#include "Engine/SkeletalMeshSocket.h"

const int8 UNARMED = -1;

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (isDirty)
	{
		isDirty = false;
		DOnInventoryChanged.ExecuteIfBound();
	}
}

void UInventoryComponent::AddItemAt(TWeakObjectPtr<UItemObject> newItemObj)
{
	AddItemAt(newItemObj, newItemObj->GetTopLeft());
}

void UInventoryComponent::AddItemAt(TWeakObjectPtr<UItemObject> newItemObj, const FTile& topLeft)
{
	newItemObj->SetTopLeft(topLeft);

	FIntPoint dim = newItemObj->GetDimensions();
	for (int i = topLeft.Y; i < topLeft.Y + dim.Y; i++)
	{
		for (int j = topLeft.X; j < topLeft.X + dim.X; j++)
		{
			const int index = TileToIndex({ j, i });
			grids[index] = newItemObj;
		}
	}
	items.Add(newItemObj, topLeft);
	isDirty = true;
}

bool UInventoryComponent::IsRoomAvailable(TWeakObjectPtr<UItemObject> newItemObj, const FTile& topLeft)
{
	FIntPoint dim = newItemObj->GetDimensions();
	for (int i = topLeft.Y; i < topLeft.Y + dim.Y; i++)
	{
		for (int j = topLeft.X; j < topLeft.X + dim.X; j++)
		{
			FTile curTile{ j, i };
			if (IsTileValid(curTile))
			{
				TPair<bool, TWeakObjectPtr<UItemObject>> result = GetItemAtIndex(TileToIndex(curTile));
				if (result.Key)
				{
					if (result.Value.IsValid() && result.Value != newItemObj)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

TPair<bool, TWeakObjectPtr<UItemObject>> UInventoryComponent::GetItemAtIndex(const int index)
{
	if (grids.IsValidIndex(index))
	{
		return { true, grids[index] };
	}
	else
	{
		return { false, nullptr };
	}
}

FTile UInventoryComponent::IndexToTile(int index)
{
	return FTile{ index % columns, index / columns };
}

bool UInventoryComponent::IsTileValid(FTile tile)
{
	return (tile.X >= 0 && tile.X < columns && tile.Y >= 0 && tile.Y < rows);
}

int UInventoryComponent::TileToIndex(FTile tile)
{
	return tile.X + tile.Y * columns;
}

void UInventoryComponent::RemoveItem(TWeakObjectPtr<UItemObject> removedItem)
{
	if (items.Find(removedItem))
	{
		items.Remove(removedItem);
		RemoveItemGrid(removedItem, removedItem->GetTopLeft());
		isDirty = true;
	}
}

void UInventoryComponent::RemoveItemGrid(TWeakObjectPtr<UItemObject> removedItem, const FTile& topLeft)
{
	FIntPoint dim = removedItem->GetDimensions();
	for (uint16 i = topLeft.Y; i < topLeft.Y + dim.Y; i++)
	{
		for (uint16 j = topLeft.X; j < topLeft.X + dim.X; j++)
		{
			const int index = TileToIndex({ j, i });
			grids[index].Reset();
		}
	}
}

void UInventoryComponent::RemoveEquipmentItem(const int slotNumber, const EEquipmentSlot slotType)
{
	switch (slotType)
	{
	case EEquipmentSlot::Weapon:
		if (equippedItems.IsValidIndex(slotNumber))
		{
			equippedItems[slotNumber].Reset();
		}
		break;
	}
}

TMap<TWeakObjectPtr<UItemObject>, FTile> UInventoryComponent::GetAllItems() const
{
	return items;
}

void UInventoryComponent::EquipItem(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor)
{
	check(equippedItems.IsValidIndex(boxNumber));
	equippedItems[boxNumber] = itemActor;
}

void UInventoryComponent::UnequipItem(TWeakObjectPtr<AItemBase> itemActor)
{
	for (uint8 i=0; i< equippedItems.Num(); i++)
	{
		if (equippedItems[i] == itemActor)
		{
			if (i == recentWeaponSlot)
			{
				recentWeaponSlot = UNARMED;
			}
			equippedItems[i].Reset();
			return;
		}
	}
}

void UInventoryComponent::Attack()
{
	if (armedWeapon.IsValid())
	{
		TWeakObjectPtr<UItemPermanent> permanentItem = Cast<UItemPermanent>(armedWeapon->GetItemObject());
		if (permanentItem.IsValid())
		{
			permanentItem->Using(armedWeapon->GetSkeletalMesh());
		}
	}
}

bool UInventoryComponent::IsWeaponUsable()
{
	if (armedWeapon.IsValid())
	{
		if (armedWeapon->GetItemType() == EItemMainType::RangedWeapon)
		{
			if (GetItemObjectOfType(EItemMainType::AmmoItem).IsValid())
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}

TWeakObjectPtr<AItemBase> UInventoryComponent::ArmRecentWeapon()
{
	if (armedWeapon.IsValid())
	{
		return nullptr;
	}

	if (recentWeaponSlot != -1)
	{
		armedWeapon = equippedItems[recentWeaponSlot];
		return armedWeapon;
	}

	for (uint8 i = 0; i < equippedItems.Num(); i++)
	{
		if (equippedItems[i].IsValid())
		{
			armedWeapon = equippedItems[i];
			recentWeaponSlot = i;
			return armedWeapon;
		}
	}
	return nullptr;
}

EWeaponType UInventoryComponent::GetCurrentWeaponType() const
{
	if (armedWeapon.IsValid())
	{
		auto permanentItem = Cast<UItemPermanent>(armedWeapon->GetItemObject());
		return StaticCast<EWeaponType>(permanentItem->GetItemSubType());
	}
	return EWeaponType::NONE;
}

void UInventoryComponent::DisarmWeapon()
{
	armedWeapon.Reset();
}

void UInventoryComponent::InitializeEquippedWeaponArr(const int size)
{
	equippedItems.Init(nullptr, size);
}

int UInventoryComponent::GetSlotNumber(TWeakObjectPtr<UItemObject> itemObj)
{
	for (uint8 i=0; i< equippedItems.Num(); i++)
	{
		if (equippedItems[i].IsValid() && equippedItems[i]->GetItemObject() == itemObj)
			return i;
	}
	return -1;
}

void UInventoryComponent::SetCharacter(TWeakObjectPtr<APlayerCharacter> character)
{
	ownerCharacter = character;
}

TWeakObjectPtr<UItemObject> UInventoryComponent::GetItemObjectOfType(const EItemMainType itemType)
{
	for (auto& pair : items)
	{
		if (pair.Key->GetItemType() == itemType)
			return pair.Key;
	}
	return nullptr;
}

void UInventoryComponent::UsingConsumableItemOfType(const EItemMainType itemType)
{
	TWeakObjectPtr<UItemConsumable> consuambleItemObj = Cast<UItemConsumable>(GetItemObjectOfType(itemType));
	if (consuambleItemObj.IsValid())
	{
		consuambleItemObj->Using();
	}
}

bool UInventoryComponent::IsAnyWeaponArmed()
{
	return armedWeapon.IsValid();
}

TWeakObjectPtr<AItemBase> UInventoryComponent::ChangeWeapon()
{
	if (recentWeaponSlot == -1)
		return nullptr;

	const uint8 size = equippedItems.Num();

	uint8 nextIndex = recentWeaponSlot, count = 0;
	while (count < size - 1)
	{
		++count;
		nextIndex = (nextIndex + 1) % size;
		if (equippedItems[nextIndex].IsValid())
		{
			recentWeaponSlot = nextIndex;
			armedWeapon = equippedItems[nextIndex];
			return armedWeapon;
		}
	}
	return nullptr;
}

void UInventoryComponent::SetRowColumn(const uint8 r, const uint8 c)
{
	rows = r;
	columns = c;
	grids.Init(nullptr, columns * rows);
}
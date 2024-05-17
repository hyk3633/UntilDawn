// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/InventoryComponent.h"
#include "Player/PlayerCharacter.h"
#include "Item/ItemObject.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject/ItemPermanent.h"
#include "GameMode/GameModeMainMap.h"
#include "Engine/SkeletalMeshSocket.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	grids.Init(nullptr, columns * rows);
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
						WLOG(TEXT("item obj"));
						return false;
					}
				}
				else
				{
					WLOG(TEXT("invalid index"));
					return false;
				}
			}
			else
			{
				WLOG(TEXT("invalid tile"));
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
		isDirty = true;
	}
}

void UInventoryComponent::RemoveEquipmentItem(const int slotNumber, const EEquipmentBox boxType)
{
	switch (boxType)
	{
	case EEquipmentBox::Weapon:
		if (equippedItems.IsValidIndex(slotNumber))
		{
			equippedItems[slotNumber] = nullptr;
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
	for (int i=0; i< equippedItems.Num(); i++)
	{
		if (equippedItems[i] == itemActor)
		{
			equippedItems[i].Reset();
			return;
		}
	}
}

void UInventoryComponent::Attack(TWeakObjectPtr<APlayerController> ownerController)
{
	if (armedWeapon.IsValid())
	{
		auto permanentItem = Cast<UItemPermanent>(armedWeapon->GetItemObject());
		permanentItem->Using(ownerController, armedWeapon->GetSkeletalMesh());
	}
}

EPermanentItemType UInventoryComponent::ArmRecentWeapon()
{
	if (armedWeapon.IsValid())
	{
		return GetCurrentWeaponType();
	}

	if (recentWeaponSlot != -1)
	{
		armedWeapon = equippedItems[recentWeaponSlot];
		armedWeapon->ActivateActor();
		return GetCurrentWeaponType();
	}

	for (int i = 0; i < equippedItems.Num(); i++)
	{
		if (equippedItems[i].IsValid())
		{
			armedWeapon = equippedItems[i];
			recentWeaponSlot = i;

			APlayerCharacter* player = Cast<APlayerCharacter>(ownerCharacter);
			const USkeletalMeshSocket* socket = player->GetMesh()->GetSocketByName(armedWeapon->GetSocketName());
			socket->AttachActor(armedWeapon.Get(), player->GetMesh());
			return GetCurrentWeaponType();
		}
	}
	return EPermanentItemType::NONE;
}

EPermanentItemType UInventoryComponent::GetCurrentWeaponType() const
{
	if (armedWeapon.IsValid())
	{
		auto permanentItem = Cast<UItemPermanent>(armedWeapon->GetItemObject());
		return permanentItem->GetPermanentItemType();
	}
	return EPermanentItemType::NONE;
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
	for (int i=0; i< equippedItems.Num(); i++)
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
		if (StaticCast<EItemMainType>(pair.Key->GetItemType()) == itemType)
			return pair.Key;
	}
	return nullptr;
}


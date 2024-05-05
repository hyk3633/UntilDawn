// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/InventoryComponent.h"
#include "Player/PlayerCharacter.h"
#include "Item/ItemObject.h"
#include "Item/ItemBase.h"
#include "Item/Weapon/ItemWeapon.h"
#include "Item/Consumable/ItemRecovery.h"
#include "GameMode/GameModeMainMap.h"
#include "Engine/SkeletalMeshSocket.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	items.Init(nullptr, columns * rows);
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

bool UInventoryComponent::TryAddItem(TWeakObjectPtr<UItemObject> newItemObj)
{
	if (newItemObj.IsValid())
	{
		for (int i = 0; i < items.Num(); i++)
		{
			if (IsRoomAvailable(newItemObj, i))
			{
				AddItemAt(newItemObj, i);
				return true;
			}
		}
		newItemObj->Rotate();
		for (int i = 0; i < items.Num(); i++)
		{
			if (IsRoomAvailable(newItemObj, i))
			{
				AddItemAt(newItemObj, i);
				return true;
			}
		}
		newItemObj->Rotate();
		return false;
	}
	else
	{
		return false;
	}
}

void UInventoryComponent::AddItemAt(TWeakObjectPtr<UItemObject> newItemObj, int topLeftIndex)
{
	newItemObj->SetTopLeftIndex(topLeftIndex);
	FTile tile = IndexToTile(topLeftIndex);
	FIntPoint dim = newItemObj->GetDimensions();
	for (int i = tile.Y; i < tile.Y + dim.Y; i++)
	{
		for (int j = tile.X; j < tile.X + dim.X; j++)
		{
			const int index = TileToIndex({ j, i });
			items[index] = newItemObj;
		}
	}
	isDirty = true;
}

bool UInventoryComponent::IsRoomAvailable(TWeakObjectPtr<UItemObject> newItemObj, int topLeftIndex)
{
	FTile tile = IndexToTile(topLeftIndex);
	FIntPoint dim = newItemObj->GetDimensions();
	for (int i = tile.Y; i < tile.Y + dim.Y; i++)
	{
		for (int j = tile.X; j < tile.X + dim.X; j++)
		{
			FTile curTile{ j, i };
			if (IsTileValid(curTile))
			{
				TPair<bool, TWeakObjectPtr<UItemObject>> result = GetItemAtIndex(TileToIndex(curTile));
				if (result.Key)
				{
					if (result.Value.IsValid())
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
	if (items.IsValidIndex(index))
	{
		return { true, items[index] };
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
	if (removedItem.IsValid())
	{
		for (int i = 0; i < items.Num(); i++)
		{
			if (items[i] == removedItem)
			{
				items[i] = nullptr;
				isDirty = true;
			}
		}
	}
}

void UInventoryComponent::RemoveEquipmentItem(const int slotNumber, const EEquipmentBox boxType)
{
	switch (boxType)
	{
	case EEquipmentBox::Weapon:
		if (equippedWeaponArr.IsValidIndex(slotNumber))
		{
			equippedWeaponArr[slotNumber] = nullptr;
		}
		break;
	}
}

void UInventoryComponent::GetAllItems(TMap<TWeakObjectPtr<UItemObject>, FTile>& itemsAll)
{
	for (int i = 0; i < items.Num(); i++)
	{
		if (items[i].IsValid() && itemsAll.Contains(items[i]) == false)
		{
			itemsAll.Add(items[i], IndexToTile(i));
		}
	}
}

void UInventoryComponent::EquipItem(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor)
{
	const EItemMainType itemType = StaticCast<EItemMainType>(itemActor->GetItemObject()->GetItemType());
	switch (itemType)
	{
		case EItemMainType::MeleeWeapon:
		case EItemMainType::RangedWeapon:
		{
			if (equippedWeaponArr.IsValidIndex(boxNumber))
			{
				equippedWeaponArr[boxNumber] = Cast<AItemWeapon>(itemActor);
			}
			break;
		}
	}
}

void UInventoryComponent::Attack(TWeakObjectPtr<APlayerController> ownerController)
{
	armedWeapon->Attack(ownerController);
}

EWeaponType UInventoryComponent::ArmRecentWeapon()
{
	if (armedWeapon.IsValid())
	{
		return GetCurrentWeaponType();
	}

	if (recentWeaponSlot != -1)
	{
		armedWeapon = equippedWeaponArr[recentWeaponSlot];
		armedWeapon->ActivateActor();
		return GetCurrentWeaponType();
	}

	for (int i = 0; i < equippedWeaponArr.Num(); i++)
	{
		if (equippedWeaponArr[i].IsValid())
		{
			armedWeapon = equippedWeaponArr[i];
			recentWeaponSlot = i;

			APlayerCharacter* player = Cast<APlayerCharacter>(GetOwner());
			const USkeletalMeshSocket* socket = player->GetMesh()->GetSocketByName(armedWeapon->GetSocketName());
			socket->AttachActor(armedWeapon.Get(), player->GetMesh());
			return GetCurrentWeaponType();
		}
	}
	return EWeaponType::NONE;
}

EWeaponType UInventoryComponent::GetCurrentWeaponType() const
{
	if (armedWeapon.IsValid())
	{
		return armedWeapon->GetWeaponType();
	}
	return EWeaponType::NONE;
}

void UInventoryComponent::DisarmWeapon()
{
	armedWeapon.Reset();
}

void UInventoryComponent::InitializeEquippedWeaponArr(const int size)
{
	equippedWeaponArr.Init(nullptr, size);
}

bool UInventoryComponent::UsingRecoveryItem()
{
	for (int i = 0; i < items.Num(); i++)
	{
		if (static_cast<EItemMainType>(items[i]->GetItemType()) == EItemMainType::RecoveryItem)
		{
			TWeakObjectPtr<AItemRecovery> recoveryItem = Cast<AItemRecovery>(GetWorld()->GetAuthGameMode<AGameModeMainMap>()->GetItemActor(items[i]->GetItemID()));
			recoveryItem->UsingItem();
			return true;
		}
	}
	return false;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/InventoryComponent.h"
#include "Item/ItemObject.h"

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

void UInventoryComponent::RemoveEquipmentItem(const EEquipmentBox boxType)
{
	switch (boxType)
	{
	case EEquipmentBox::RangedWeapon1:
		rangedWeapon1 = nullptr;
		break;
	case EEquipmentBox::RangedWeapon2:
		rangedWeapon2 = nullptr;
		break;
	case EEquipmentBox::MeleeWeapon:
		meleeWeapon = nullptr;
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


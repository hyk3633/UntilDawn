// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetItemInventory.h"
#include "UI/Main/WidgetEquipmentWindow.h"
#include "UI/Main/WidgetInventoryGrid.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "GameMode/GameModeMainMap.h"
#include "GameSystem/InventoryComponent.h"
#include "Item/ItemObject.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UWidgetItemInventory::InitializeWidget()
{
	inventoryComponent = Cast<APlayerControllerMainMap>(GetOwningPlayer())->GetInventoryComponent();
	InventoryGrid->InitializeWidget(inventoryComponent, tileSize);
	EquipmentWindow->InitializeWidget(inventoryComponent, tileSize);
}

void UWidgetItemInventory::ItemDrop(UDragDropOperation* operation)
{
	if (IsValid(operation) == false || IsValid(operation->Payload) == false)
		return;

	if (InventoryGrid->GetIsCursorInArea() == false && EquipmentWindow->GetIsCursorInArea() == false)
	{
		TWeakObjectPtr<UItemObject> itemObj = Cast<UItemObject>(operation->Payload);
		APlayerControllerMainMap* playerController = Cast<APlayerControllerMainMap>(GetOwningPlayer());
		check(playerController);
		const int slotNumber = inventoryComponent->GetSlotNumber(itemObj);
		if (slotNumber != -1)
		{
			playerController->DropEquippedItem(itemObj->GetItemID());
		}
		else
		{
			playerController->DropInventoryItem(itemObj->GetItemID());
		}
	}
}

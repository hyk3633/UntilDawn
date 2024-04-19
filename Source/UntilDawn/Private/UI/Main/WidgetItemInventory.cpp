// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetItemInventory.h"
#include "UI/Main/WidgetEquipmentWindow.h"
#include "UI/Main/WidgetInventoryGrid.h"
#include "Player/PlayerCharacter.h"
#include "GameMode/GameModeMainMap.h"
#include "GameSystem/InventoryComponent.h"
#include "Item/ItemObject.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UWidgetItemInventory::InitializeWidget()
{
	inventoryComponent = Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventoryComponent();
	InventoryGrid->Init(inventoryComponent, tileSize);
}

void UWidgetItemInventory::ItemDrop(UDragDropOperation* operation)
{
	if (inventoryComponent->IsTileValid({ InventoryGrid->GetDragged().X, InventoryGrid->GetDragged().Y }) == false || isCursorInWidgetArea)
	{
		TWeakObjectPtr<UItemObject> payload = Cast<UItemObject>(operation->Payload);
		GetWorld()->GetAuthGameMode<AGameModeMainMap>()->DropItem(payload);
	}
	isCursorInWidgetArea = false;
}

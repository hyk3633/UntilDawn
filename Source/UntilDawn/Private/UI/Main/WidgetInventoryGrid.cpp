// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetInventoryGrid.h"
#include "GameSystem/InventoryComponent.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "UI/Main/WidgetItemGrid.h"
#include "Item/ItemObject.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetInputLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Slate/SlateBrushAsset.h"

void UWidgetInventoryGrid::InitializeWidget(UInventoryComponent* invComp, float size)
{
	inventoryComponent = invComp;
	inventoryComponent->DOnInventoryChanged.BindUFunction(this, FName("Refresh"));

	tileSize = size;
	const float sizeX = inventoryComponent->GetColumns() * tileSize;
	const float sizeY = inventoryComponent->GetRows() * tileSize;

	UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder)->SetSize(FVector2D(sizeX, sizeY));

	CreateLineSegments();

	Refresh();
}

void UWidgetInventoryGrid::CreateLineSegments()
{
	const int columns = inventoryComponent->GetColumns();
	float x = 0;
	const float endY = inventoryComponent->GetRows() * tileSize;
	for (int i = 0; i < columns; i++)
	{
		x = i * tileSize;
		lines.Add(FLine{ {x,0},{x,endY} });
	}

	const int rows = inventoryComponent->GetRows();
	float y = 0;
	const float endX = inventoryComponent->GetColumns() * tileSize;
	for (int i = 0; i < rows; i++)
	{
		y = i * tileSize;
		lines.Add(FLine{ {0,y},{endX,y} });
	}
}

void UWidgetInventoryGrid::Refresh()
{
	GridCanvasPanel->ClearChildren();
	TMap<TWeakObjectPtr<UItemObject>, FTile> itemsAll;
	inventoryComponent->GetAllItems(itemsAll);
	for (auto& kv : itemsAll)
	{
		UWidgetItemGrid* itemWidget = CreateWidget<UWidgetItemGrid>(GetOwningPlayer(), itemWidgetClass);
		itemWidget->SetItem(kv.Key);
		itemWidget->SetTileSize(tileSize);
		itemWidget->DOnRemoved.BindUFunction(this, FName("OnItemRemoved"));
		itemWidget->Refresh();

		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(GridCanvasPanel->AddChild(itemWidget));
		slot->SetAutoSize(true);
		slot->SetPosition(FVector2D{ kv.Value.X * tileSize, kv.Value.Y * tileSize });
	}
}

void UWidgetInventoryGrid::OnItemRemoved(UItemObject* removedItem)
{
	inventoryComponent->RemoveItem(removedItem);
}

bool UWidgetInventoryGrid::IsRoomAvailableForPayload(UItemObject* payloadItem) const
{
	if (IsValid(payloadItem))
	{
		const int index = inventoryComponent->TileToIndex({ draggedItemTopLeftTile.X, draggedItemTopLeftTile.Y });
		return inventoryComponent->IsRoomAvailable(payloadItem, index);
	}
	return false;
}

bool UWidgetInventoryGrid::MousePositionOfTileRightLeft(float mousePositionX)
{
	return (FMath::TruncToInt(mousePositionX) % FMath::TruncToInt(tileSize)) > (tileSize / 2.f);
}

bool UWidgetInventoryGrid::MousePositionOfTileUpDown(float mousePositionY)
{
	return (FMath::TruncToInt(mousePositionY) % FMath::TruncToInt(tileSize)) > (tileSize / 2.f);
}

bool UWidgetInventoryGrid::OnPaintCalled(FPaintContext context) const
{
	for (auto& line : lines)
	{
		FVector2D topLeft = USlateBlueprintLibrary::GetLocalTopLeft(GridBorder->GetCachedGeometry());
		UWidgetBlueprintLibrary::DrawLine
		(
			context,
			line.start + topLeft,
			line.end + topLeft,
			FLinearColor{ 0.5f,0.5f,0.5f,0.5f }
		);
	}
	if (isCursorInArea && UWidgetBlueprintLibrary::IsDragDropping())
	{
		TWeakObjectPtr<UItemObject> item = GetPayload(UWidgetBlueprintLibrary::GetDragDroppingContent());
		FVector2D boxSize = item->GetDimensions() * tileSize;
		FLinearColor color{ 1.f,0.f,0.f,0.25f };
		if (IsRoomAvailableForPayload(item.Get()))
		{
			color = { 0.f,1.f,0.f,0.25f };
		}
		UWidgetBlueprintLibrary::DrawBox
		(
			context,
			draggedItemTopLeftTile * tileSize,
			boxSize,
			slateBrush,
			color
		);
	}
	return true;
}

void UWidgetInventoryGrid::OnDropCalled(UDragDropOperation* operation)
{
	APlayerControllerMainMap* playerController = Cast<APlayerControllerMainMap>(GetOwningPlayer());
	check(playerController);

	TWeakObjectPtr<UItemObject> itemObj = GetPayload(operation);

	const int slotNumber = inventoryComponent->GetSlotNumber(itemObj);
	if (slotNumber != -1)
	{
		playerController->NotifyToServerUnequipItem(itemObj->GetItemID(), { draggedItemTopLeftTile.X, draggedItemTopLeftTile.Y });
	}
	else
	{
		playerController->NotifyToServerUpdateItemGridPoint(itemObj->GetItemID(), draggedItemTopLeftTile.X, draggedItemTopLeftTile.Y, itemObj->IsRotated());
	}
}

bool UWidgetInventoryGrid::OnDragOverCalled(FPointerEvent pointEvent, FGeometry geometry, UDragDropOperation* operation)
{
	FVector2D coordinate = UKismetInputLibrary::PointerEvent_GetScreenSpacePosition(pointEvent);
	FVector2D mousePosition = USlateBlueprintLibrary::AbsoluteToLocal(geometry, coordinate);
	const bool bRight = MousePositionOfTileRightLeft(mousePosition.X);
	const bool bDown = MousePositionOfTileUpDown(mousePosition.Y);
	TWeakObjectPtr<UItemObject> payloadItem = Cast<UItemObject>(operation->Payload);
	FIntPoint dim = payloadItem->GetDimensions();

	draggedItemTopLeftTile =
	{
		(int)FMath::TruncToInt(mousePosition.X / tileSize) - FMath::Max(dim.X - bRight, 0) / 2,
		(int)FMath::TruncToInt(mousePosition.Y / tileSize) - FMath::Max(dim.Y - bDown, 0) / 2
	};
	return true;
}

void UWidgetInventoryGrid::OnPreviewKeyDownCalled(const bool isKeyRight)
{
	if (isKeyRight)
	{
		TWeakObjectPtr<UItemObject> payloadItem = GetPayload(UWidgetBlueprintLibrary::GetDragDroppingContent());
		if (payloadItem.IsValid())
		{
			payloadItem->Rotate();
			UWidgetItemGrid* itemWidget = Cast<UWidgetItemGrid>(UWidgetBlueprintLibrary::GetDragDroppingContent()->DefaultDragVisual);
			itemWidget->Refresh();
		}
	}
}

UItemObject* UWidgetInventoryGrid::GetPayload(UDragDropOperation* operation) const
{
	if (IsValid(operation))
	{
		return Cast<UItemObject>(operation->Payload);
	}
	return nullptr;
}

void UWidgetInventoryGrid::SetCursorInArea(const bool bIn)
{
	isCursorInArea = bIn;
}

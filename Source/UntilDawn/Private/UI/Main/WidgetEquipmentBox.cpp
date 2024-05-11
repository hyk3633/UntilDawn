// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetEquipmentBox.h"
#include "UI/Main/WidgetDragVisual.h"
#include "Item/ItemObject.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "GameMode/GameModeMainMap.h"
#include "GameSystem/ItemManager.h"
#include "GameSystem/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Slate/SlateBrushAsset.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UWidgetEquipmentBox::InitializeWidget(const int num, UInventoryComponent* invComp, TSubclassOf<UWidgetDragVisual> dragVisClass, const EItemMainType newItemYype, const EEquipmentBox newBoxType, const float size)
{
	number = num;
	inventoryComponent = invComp;
	dragVisualClass = dragVisClass;
	dragVisual = CreateWidget<UWidgetDragVisual>(GetOwningPlayer(), dragVisualClass);
	dragVisual->SetVisibility(ESlateVisibility::Hidden);
	equipmentType = newItemYype;
	boxType = newBoxType;
	tileSize = size;
}

void UWidgetEquipmentBox::OnDropCalled(UDragDropOperation* operation)
{
	TWeakObjectPtr<UItemObject> newItemObj = Cast<UItemObject>(operation->Payload);
	check(newItemObj.IsValid());
	if (static_cast<EItemMainType>(newItemObj->GetItemType()) == equipmentType)
	{
		// 플레이어 컨트롤러 호출
		APlayerControllerMainMap* playerController = Cast<APlayerControllerMainMap>(GetOwningPlayer());
		check(playerController);
		playerController->SendItemInfoToEquip(newItemObj->GetItemID(), number);
	}
	else
	{
		inventoryComponent->AddItemAt(newItemObj, newItemObj->GetTopLeftIndex());
	}
}

void UWidgetEquipmentBox::OnRemoved()
{
	DOnEquipmentRemoved.ExecuteIfBound(number, boxType);
}

void UWidgetEquipmentBox::Refresh()
{
	FIntPoint dim = itemObj->GetRotatedDimensions();
	widgetSize = FVector2D(dim.X * tileSize, dim.Y * tileSize);
	BackGroundSizeBox->SetWidthOverride(widgetSize.X);
	BackGroundSizeBox->SetHeightOverride(widgetSize.Y);
	UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage)->SetSize(widgetSize);

	FIntPoint originDim = itemObj->GetDimensions();
	dragVisual->InitilizeWidget(FVector2D(originDim.X * tileSize, originDim.Y * tileSize), GetOriginIconImage());
}

void UWidgetEquipmentBox::OnDragDetectedCall()
{
	//OnRemoved();
	ItemImage->SetBrush(FSlateBrush());
	BackGroundSizeBox->SetWidthOverride(0);
	BackGroundSizeBox->SetHeightOverride(0);
	UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage)->SetSize(FVector2D(0,0));
}

void UWidgetEquipmentBox::Equip(TWeakObjectPtr<UItemObject> newItemObj)
{
	itemObj = newItemObj;
	Refresh();
	ItemImage->SetBrush(GetIconImage());
}

UItemObject* UWidgetEquipmentBox::GetItem() const
{
	return itemObj.Get();
}

FSlateBrush UWidgetEquipmentBox::GetIconImage()
{
	return UWidgetBlueprintLibrary::MakeBrushFromMaterial
	(
		itemObj->GetRotatedIcon(),
		FMath::TruncToInt(widgetSize.Y),
		FMath::TruncToInt(widgetSize.X)
	);
}

FSlateBrush UWidgetEquipmentBox::GetOriginIconImage()
{
	return UWidgetBlueprintLibrary::MakeBrushFromMaterial
	(
		itemObj->GetIcon(),
		FMath::TruncToInt(widgetSize.Y),
		FMath::TruncToInt(widgetSize.X)
	);
}

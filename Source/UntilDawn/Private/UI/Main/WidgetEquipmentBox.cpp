// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetEquipmentBox.h"
#include "UI/Main/WidgetDragVisual.h"
#include "Item/ItemObject.h"
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

void UWidgetEquipmentBox::InitializeWidget(UInventoryComponent* invComp, const EItemMainType type, const float size)
{
	inventoryComponent = invComp;
	dragVisual = CreateWidget<UWidgetDragVisual>(GetOwningPlayer(), dragVisualClass);
	dragVisual->SetVisibility(ESlateVisibility::Hidden);
	equipmentType = type;
	tileSize = size;
}

void UWidgetEquipmentBox::OnDropCalled(UDragDropOperation* operation)
{
	TWeakObjectPtr<UItemObject> newItemObj = Cast<UItemObject>(operation->Payload);
	if (static_cast<EItemMainType>(newItemObj->GetItemType()) == equipmentType)
	{
		itemObj = newItemObj;
		Refresh();
		ItemImage->SetBrush(GetIconImage());

		AGameModeMainMap* gameMode = Cast<AGameModeMainMap>(UGameplayStatics::GetGameMode(this));
		TWeakObjectPtr<AItemBase> item = gameMode->GetItemActor(itemObj);
		inventoryComponent->EquipItem(number, boxType, item);
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
	OnRemoved();
	ItemImage->SetBrush(FSlateBrush());
	BackGroundSizeBox->SetWidthOverride(0);
	BackGroundSizeBox->SetHeightOverride(0);
	UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage)->SetSize(FVector2D(0,0));
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

void UWidgetEquipmentBox::SetEquipmentAndBoxType(const EEquipmentBox newBoxType)
{
	boxType = newBoxType;
}

void UWidgetEquipmentBox::SetNumber(const int num)
{
	number = num;
}

void UWidgetEquipmentBox::SetDragVisualClass(TSubclassOf<UWidgetDragVisual> dragVisClass)
{
	dragVisualClass = dragVisClass;
}

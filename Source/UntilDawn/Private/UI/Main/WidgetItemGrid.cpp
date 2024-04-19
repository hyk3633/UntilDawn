// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetItemGrid.h"
#include "Item/ItemObject.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Slate/SlateBrushAsset.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UWidgetItemGrid::OnRemoved()
{
    DOnRemoved.ExecuteIfBound(itemObj.Get());
}

void UWidgetItemGrid::Refresh()
{
	FIntPoint dim = itemObj->GetDimensions();
	widgetSize = FVector2D(dim.X * tileSize, dim.Y * tileSize);
	BackGroundSizeBox->SetWidthOverride(widgetSize.X);
	BackGroundSizeBox->SetHeightOverride(widgetSize.Y);
	UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage)->SetSize(widgetSize);
}

void UWidgetItemGrid::OnDragDetectedCall()
{
	OnRemoved();
	RemoveFromParent();
}

UItemObject* UWidgetItemGrid::GetItem() const
{
	return itemObj.Get();
}

FSlateBrush UWidgetItemGrid::GetIconImage()
{
	return UWidgetBlueprintLibrary::MakeBrushFromMaterial
	(
		itemObj->GetIcon(),
		FMath::TruncToInt(widgetSize.X),
		FMath::TruncToInt(widgetSize.Y)
	);
}

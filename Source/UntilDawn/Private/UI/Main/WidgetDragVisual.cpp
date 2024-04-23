// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetDragVisual.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UWidgetDragVisual::InitilizeWidget(const FVector2D& widgetSize, const FSlateBrush& brush)
{
	BackGroundSizeBox->SetWidthOverride(widgetSize.X);
	BackGroundSizeBox->SetHeightOverride(widgetSize.Y);
	UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage)->SetSize(widgetSize);
	ItemImage->SetBrush(brush);
}

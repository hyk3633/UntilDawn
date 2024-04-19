// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetMainInterface.h"
#include "UI/Main/WidgetItemInventory.h"

void UWidgetMainInterface::InitializeWidget()
{
	ItemInventory->InitializeWidget();
	ItemInventory->SetVisibility(ESlateVisibility::Hidden);

}

void UWidgetMainInterface::ToggleInventoryUI()
{
	if (ItemInventory->IsVisible())
	{
		ItemInventory->SetVisibility(ESlateVisibility::Hidden);

		FInputModeGameOnly inputMode;
		GetOwningPlayer()->SetInputMode(inputMode);
		GetOwningPlayer()->bShowMouseCursor = false;
	}
	else
	{
		ItemInventory->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI inputMode;
		inputMode.SetWidgetToFocus(ItemInventory->GetCachedWidget());
		GetOwningPlayer()->SetInputMode(inputMode);
		GetOwningPlayer()->bShowMouseCursor = true;
	}
}

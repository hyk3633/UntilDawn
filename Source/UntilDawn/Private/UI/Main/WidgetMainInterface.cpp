// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetMainInterface.h"
#include "UI/Main/WidgetItemInventory.h"
#include "Components/ProgressBar.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Player/PlayerCharacter.h"

void UWidgetMainInterface::InitializeWidget()
{
	ItemInventory->InitializeWidget();
	ItemInventory->SetVisibility(ESlateVisibility::Hidden);

	playerCharacter = Cast<APlayerCharacter>(GetOwningPlayer()->GetPawn());
	playerCharacter->DHealthChanged.BindUFunction(this, FName("OnCharacterHealthChanged"));
	HealthBar->SetPercent(playerCharacter->GetHealthPercentage());

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

void UWidgetMainInterface::OnCharacterHealthChanged(const float percentage)
{
	HealthBar->SetPercent(percentage);
}

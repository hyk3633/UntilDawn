// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetMainInterface.h"
#include "UI/Main/WidgetItemInventory.h"
#include "Components/ProgressBar.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Image.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"

void UWidgetMainInterface::InitializeWidget()
{
	ItemInventory->InitializeWidget();
	ItemInventory->SetVisibility(ESlateVisibility::Hidden);

	playerController = Cast<APlayerControllerMainMap>(GetOwningPlayer());
	playerController->DHealthChanged.BindUFunction(this, FName("OnCharacterHealthChanged"));
	playerController->DWeaponArmed.BindUFunction(this, FName("OnChangeEquippedWeapon"));
	HealthBar->SetPercent(1.f);
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

void UWidgetMainInterface::OnChangeEquippedWeapon(UMaterialInstance* matInst)
{
	EquipmentQuickSlotImage->SetBrushFromMaterial(matInst);
}

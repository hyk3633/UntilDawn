// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetMainInterface.h"
#include "UI/Main/WidgetItemInventory.h"
#include "Components/ProgressBar.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"
#include "Item/ItemObject.h"
#include "Item/ItemObject/ItemProjectileWeapon.h"

void UWidgetMainInterface::InitializeWidget()
{
	ItemInventory->InitializeWidget();
	ItemInventory->SetVisibility(ESlateVisibility::Hidden);

	playerController = Cast<APlayerControllerMainMap>(GetOwningPlayer());
	playerController->DHealthChanged.BindUFunction(this, FName("OnCharacterHealthChanged"));
	playerController->onWeaponArmed.BindUFunction(this, FName("OnChangeEquippedWeapon"));

	HealthProgressBar->SetPercent(1.f);
	EquipmentQuickSlotImage->SetVisibility(ESlateVisibility::Hidden);
	AmmoAmountStatus->SetVisibility(ESlateVisibility::Hidden);
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
	HealthProgressBar->SetPercent(percentage);
}

void UWidgetMainInterface::OnChangeEquippedWeapon(UItemObject* itemObj)
{
	TWeakObjectPtr<UItemObject> itemObjPtr = itemObj;
	if (itemObjPtr.IsValid())
	{
		EquipmentQuickSlotImage->SetVisibility(ESlateVisibility::Visible);
		EquipmentQuickSlotImage->SetBrushFromMaterial(itemObjPtr->GetRotatedIcon());

		SetRangedWeaponUI(itemObjPtr);
	}
	else
	{
		EquipmentQuickSlotImage->SetVisibility(ESlateVisibility::Hidden);
		EquipmentQuickSlotImage->SetBrushFromMaterial(nullptr);
	}
}

void UWidgetMainInterface::SetRangedWeaponUI(TWeakObjectPtr<UItemObject> itemObj)
{
	if (projectileWeaponObj.IsValid())
	{
		AmmoAmountStatus->SetVisibility(ESlateVisibility::Hidden);
		projectileWeaponObj->onFireWeapon.Unbind();
		projectileWeaponObj.Reset();
	}
	TWeakObjectPtr<UItemProjectileWeapon> newProjectileWeaponObj = Cast<UItemProjectileWeapon>(itemObj);
	if (newProjectileWeaponObj.IsValid())
	{
		newProjectileWeaponObj->onFireWeapon.BindUFunction(this, FName("AmmoStatusChanged"));
		projectileWeaponObj = newProjectileWeaponObj;

		const auto magazineAmount = newProjectileWeaponObj->GetMagazineAmount();
		const auto remainedAmmoAmount = newProjectileWeaponObj->GetLoadedAmmoAmount();

		MagazineAmountText->SetText(FText::FromString(FString::FromInt(magazineAmount)));
		LoadedAmmoAmountText->SetText(FText::FromString(FString::FromInt(remainedAmmoAmount)));
		AmmoAmountProgressBar->SetPercent(StaticCast<float>(remainedAmmoAmount) / magazineAmount);

		AmmoAmountStatus->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWidgetMainInterface::AmmoStatusChanged(const uint16 remainedAmount)
{
	if (projectileWeaponObj.IsValid())
	{
		const auto magazineAmount = projectileWeaponObj->GetMagazineAmount();
		const auto remainedAmmoAmount = projectileWeaponObj->GetLoadedAmmoAmount();

		MagazineAmountText->SetText(FText::FromString(FString::FromInt(magazineAmount)));
		LoadedAmmoAmountText->SetText(FText::FromString(FString::FromInt(remainedAmmoAmount)));
		AmmoAmountProgressBar->SetPercent(StaticCast<float>(remainedAmmoAmount) / StaticCast<float>(magazineAmount));
	}
	else
	{
		projectileWeaponObj.Reset();
	}
}

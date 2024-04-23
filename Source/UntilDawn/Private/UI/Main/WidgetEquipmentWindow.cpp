// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetEquipmentWindow.h"
#include "UI/Main/WidgetEquipmentBox.h"
#include "GameSystem/InventoryComponent.h"

void UWidgetEquipmentWindow::InitializeWidget(UInventoryComponent* invComp, const float size)
{
	inventoryComponent = invComp;

	RangedWeaponBox1->SetEquipmentAndBoxType(EItemMainType::RangedWeapon, EEquipmentBox::RangedWeapon1);
	RangedWeaponBox1->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
	RangedWeaponBox1->InitializeWidget(invComp, size);

	RangedWeaponBox2->SetEquipmentAndBoxType(EItemMainType::RangedWeapon, EEquipmentBox::RangedWeapon2);
	RangedWeaponBox2->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
	RangedWeaponBox2->InitializeWidget(invComp, size);

	MeleeWeaponBox->SetEquipmentAndBoxType(EItemMainType::MeleeWeapon, EEquipmentBox::MeleeWeapon);
	MeleeWeaponBox->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
	MeleeWeaponBox->InitializeWidget(invComp, size);
}

void UWidgetEquipmentWindow::SetCursorInArea(const bool bIn)
{
	isCursorInArea = bIn;
}

void UWidgetEquipmentWindow::OnItemRemoved(EEquipmentBox boxType)
{
	inventoryComponent->RemoveEquipmentItem(boxType);
}

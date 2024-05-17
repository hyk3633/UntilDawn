// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetEquipmentWindow.h"
#include "UI/Main/WidgetEquipmentBox.h"
#include "Item/ItemObject.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "GameSystem/InventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UWidgetEquipmentWindow::InitializeWidget(UInventoryComponent* invComp, const float size)
{
	const int arrSize = equipmentTypes.Num();

	inventoryComponent = invComp;
	inventoryComponent->InitializeEquippedWeaponArr(arrSize);

	APlayerControllerMainMap* playerController = Cast<APlayerControllerMainMap>(GetOwningPlayer());
	playerController->DEquipItem.BindUFunction(this, FName("EquipItemToBox"));

	for (int i = 0; i < arrSize; i++)
	{
		UWidgetEquipmentBox* equipmentBox = CreateWidget<UWidgetEquipmentBox>(GetOwningPlayer(), equipmentBoxClass);
		UUniformGridSlot* gridSlot = EquipmentPanel->AddChildToUniformGrid(equipmentBox, i);
		gridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		gridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		
		equipmentBox->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
		equipmentBox->InitializeWidget(i, invComp, dragVisualClass, equipmentTypes[i], EEquipmentBox::Weapon, size);
		equipmentBox->Padding = FMargin(5, 5);

		equipmentBoxArr.Add(equipmentBox);
	}
}

void UWidgetEquipmentWindow::SetCursorInArea(const bool bIn)
{
	isCursorInArea = bIn;
}

void UWidgetEquipmentWindow::EquipItemToBox(UItemObject* itemObj, const int boxNumber)
{
	if (equipmentBoxArr.IsValidIndex(boxNumber))
	{
		equipmentBoxArr[boxNumber]->Equip(itemObj);
	}
}

void UWidgetEquipmentWindow::OnItemRemoved(const int slotNumber, EEquipmentBox boxType)
{
	inventoryComponent->RemoveEquipmentItem(slotNumber, boxType);
}

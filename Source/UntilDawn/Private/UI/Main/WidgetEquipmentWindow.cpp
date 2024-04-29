// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetEquipmentWindow.h"
#include "UI/Main/WidgetEquipmentBox.h"
#include "GameSystem/InventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UWidgetEquipmentWindow::InitializeWidget(UInventoryComponent* invComp, const float size)
{
	const int arrSize = equipmentTypeArr.Num();

	inventoryComponent = invComp;
	inventoryComponent->InitializeEquippedWeaponArr(arrSize);

	for (int i = 0; i < arrSize; i++)
	{
		UWidgetEquipmentBox* equipmentBox = CreateWidget<UWidgetEquipmentBox>(GetOwningPlayer(), equipmentBoxClass);
		UUniformGridSlot* gridSlot = EquipmentPanel->AddChildToUniformGrid(equipmentBox, i);
		gridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		gridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		
		equipmentBox->SetEquipmentAndBoxType(EEquipmentBox::Weapon);
		equipmentBox->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
		equipmentBox->InitializeWidget(invComp, equipmentTypeArr[i], size);
		equipmentBox->SetNumber(i);
		equipmentBox->SetDragVisualClass(dragVisualClass);
		equipmentBox->Padding = FMargin(5, 5);

		equipmentBoxMap.Add(equipmentBox);
	}
}

void UWidgetEquipmentWindow::SetCursorInArea(const bool bIn)
{
	isCursorInArea = bIn;
}

void UWidgetEquipmentWindow::OnItemRemoved(const int slotNumber, EEquipmentBox boxType)
{
	inventoryComponent->RemoveEquipmentItem(slotNumber, boxType);
}

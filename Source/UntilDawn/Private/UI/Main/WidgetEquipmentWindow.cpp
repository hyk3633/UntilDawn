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
	const int arrSize = weaponTypes.Num();

	inventoryComponent = invComp;
	inventoryComponent->InitializeEquippedWeaponArr(arrSize + 4);

	APlayerControllerMainMap* playerController = Cast<APlayerControllerMainMap>(GetOwningPlayer());
	playerController->DEquipItem.BindUFunction(this, FName("EquipItemToBox"));

	for (int i = 0; i < arrSize; i++)
	{
		UWidgetEquipmentBox* equipmentBox = CreateWidget<UWidgetEquipmentBox>(GetOwningPlayer(), equipmentBoxClass);
		UUniformGridSlot* gridSlot = EquipmentPanel->AddChildToUniformGrid(equipmentBox, i);
		gridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		gridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		
		equipmentBox->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
		equipmentBox->InitializeWidget(i, invComp, dragVisualClass, weaponTypes[i].mainType, StaticCast<uint8>(weaponTypes[i].subType), weaponTypes[i].slotType, size);
		equipmentBox->Padding = FMargin(5, 5);
	
		equipmentSlots.Add(equipmentBox);
	}

	HeadSlot->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
	HeadSlot->InitializeWidget(arrSize + StaticCast<uint8>(EArmourSlot::Head), invComp, dragVisualClass, EItemMainType::ArmourItem, StaticCast<uint8>(EArmourSlot::Head), EEquipmentSlot::Armour, size);
	equipmentSlots.Add(HeadSlot);

	TopSlot->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
	TopSlot->InitializeWidget(arrSize + StaticCast<uint8>(EArmourSlot::Top), invComp, dragVisualClass, EItemMainType::ArmourItem, StaticCast<uint8>(EArmourSlot::Top), EEquipmentSlot::Armour, size);
	equipmentSlots.Add(TopSlot);

	BottomSlot->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
	BottomSlot->InitializeWidget(arrSize + StaticCast<uint8>(EArmourSlot::Bottom), invComp, dragVisualClass, EItemMainType::ArmourItem, StaticCast<uint8>(EArmourSlot::Bottom), EEquipmentSlot::Armour, size);
	equipmentSlots.Add(BottomSlot);

	FootSlot->DOnEquipmentRemoved.BindUFunction(this, FName("OnItemRemoved"));
	FootSlot->InitializeWidget(arrSize + StaticCast<uint8>(EArmourSlot::Foot), invComp, dragVisualClass, EItemMainType::ArmourItem, StaticCast<uint8>(EArmourSlot::Foot), EEquipmentSlot::Armour, size);
	equipmentSlots.Add(FootSlot);
}

void UWidgetEquipmentWindow::SetCursorInArea(const bool bIn)
{
	isCursorInArea = bIn;
}

void UWidgetEquipmentWindow::EquipItemToBox(UItemObject* itemObj, const int slotNumber)
{
	if (equipmentSlots.IsValidIndex(slotNumber))
	{
		equipmentSlots[slotNumber]->Equip(itemObj);
	}
}

void UWidgetEquipmentWindow::OnItemRemoved(const int slotNumber, EEquipmentSlot slotType)
{
	inventoryComponent->RemoveEquipmentItem(slotNumber, slotType);
}

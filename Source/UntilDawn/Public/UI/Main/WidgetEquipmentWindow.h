// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/ItemType.h"
#include "Enums/EquipmentSlot.h"
#include "Enums/ArmourSlot.h"
#include "Enums/WeaponType.h"
#include "WidgetEquipmentWindow.generated.h"

/**
 * 
 */

class UInventoryComponent;
class UWidgetEquipmentBox;
class UItemObject;
class UUniformGridPanel;
class UWidgetDragVisual;

USTRUCT(BlueprintType)
struct FWeaponTypeInfo
{
	GENERATED_BODY()

public:

	FWeaponTypeInfo() {}

	FWeaponTypeInfo(EItemMainType mType, EWeaponType sType, EEquipmentSlot slType) : mainType(mType), subType(sType), slotType(slType) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemMainType mainType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType subType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentSlot slotType;

};

UCLASS()
class UNTILDAWN_API UWidgetEquipmentWindow : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeWidget(UInventoryComponent* invComp, const float size);

	UFUNCTION(BlueprintCallable)
	void SetCursorInArea(const bool bIn);

	FORCEINLINE int GetIsCursorInArea() const { return isCursorInArea; }

protected:

	UFUNCTION()
	void EquipItemToBox(UItemObject* itemObj, const int slotNumber);

	UFUNCTION()
	void OnItemRemoved(const int slotNumber, EEquipmentSlot slotType);

private:

	UPROPERTY()
	UInventoryComponent* inventoryComponent;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UUniformGridPanel* EquipmentPanel;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetEquipmentBox* HeadSlot;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetEquipmentBox* TopSlot;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetEquipmentBox* BottomSlot;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetEquipmentBox* FootSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidgetEquipmentBox> equipmentBoxClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidgetDragVisual> dragVisualClass;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	bool isCursorInArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "5", BindWidget))
	TArray<FWeaponTypeInfo> weaponTypes;

	UPROPERTY()
	TArray<UWidgetEquipmentBox*> equipmentSlots;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/PermanentItemType.h"
#include "Enums/EquipmentBox.h"
#include "WidgetEquipmentWindow.generated.h"

/**
 * 
 */

class UInventoryComponent;
class UWidgetEquipmentBox;
class UItemObject;
class UUniformGridPanel;
class UWidgetDragVisual;

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
	void EquipItemToBox(UItemObject* itemObj, const int boxNumber);

	UFUNCTION()
	void OnItemRemoved(const int slotNumber, EEquipmentBox boxType);

private:

	UPROPERTY()
	UInventoryComponent* inventoryComponent;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UUniformGridPanel* EquipmentPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidgetEquipmentBox> equipmentBoxClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidgetDragVisual> dragVisualClass;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	bool isCursorInArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "5", BindWidget))
	TArray<EPermanentItemType> equipmentTypeArr;

	UPROPERTY()
	TArray<UWidgetEquipmentBox*> equipmentBoxArr;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/ItemType.h"
#include "Enums/EquipmentBox.h"
#include "WidgetEquipmentWindow.generated.h"

/**
 * 
 */

class UInventoryComponent;
class UWidgetEquipmentBox;
class UItemObject;

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
	void OnItemRemoved(EEquipmentBox boxType);

private:

	UPROPERTY()
	UInventoryComponent* inventoryComponent;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetEquipmentBox* RangedWeaponBox1;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetEquipmentBox* RangedWeaponBox2;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetEquipmentBox* MeleeWeaponBox;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	bool isCursorInArea;
	
};

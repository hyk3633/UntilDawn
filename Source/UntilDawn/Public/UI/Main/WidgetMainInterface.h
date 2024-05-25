// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMainInterface.generated.h"

/**
 * 
 */

class UWidgetItemInventory;
class UProgressBar;
class UUniformGridPanel;
class APlayerControllerMainMap;
class UImage;

UCLASS()
class UNTILDAWN_API UWidgetMainInterface : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeWidget();

	void ToggleInventoryUI();

protected:

	UFUNCTION()
	void OnCharacterHealthChanged(const float percentage);

	UFUNCTION()
	void OnChangeEquippedWeapon(UMaterialInstance* matInst);

private:
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetItemInventory* ItemInventory;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* EquipmentQuickSlotImage;

	UPROPERTY()
	TWeakObjectPtr<APlayerControllerMainMap> playerController;

};

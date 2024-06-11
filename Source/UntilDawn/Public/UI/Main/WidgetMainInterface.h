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
class UOverlay;
class UTextBlock;
class UItemObject;
class UItemProjectileWeapon;

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
	void OnCharacterStaminaChanged(const float percentage);

	UFUNCTION()
	void OnChangeEquippedWeapon(UItemObject* itemObj);

	void SetRangedWeaponUI(TWeakObjectPtr<UItemObject> itemObj);

	UFUNCTION()
	void AmmoStatusChanged(const uint16 remainedAmount);

private:
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetItemInventory* ItemInventory;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UProgressBar* StaminaProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* EquipmentQuickSlotImage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UOverlay* AmmoAmountStatus;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UProgressBar* AmmoAmountProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* LoadedAmmoAmountText;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* MagazineAmountText;

	TWeakObjectPtr<APlayerControllerMainMap> playerController;

	TWeakObjectPtr<UItemProjectileWeapon> projectileWeaponObj;

};

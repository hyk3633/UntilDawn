// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Enums/ItemType.h"
#include "../../Enums/WeaponType.h"
#include "../../Enums/EquipmentBox.h"
#include "WidgetEquipmentBox.generated.h"

/**
 * 
 */

class UDragDropOperation;
class USizeBox;
class UBorder;
class UImage;
class UItemObject;
class UWidgetDragVisual;
class UInventoryComponent;

DECLARE_DELEGATE_TwoParams(DelegateOnEquipmentRemoved, int slotNumber, EEquipmentBox boxType);

UCLASS()
class UNTILDAWN_API UWidgetEquipmentBox : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget(UInventoryComponent* invComp, const EItemMainType type, const float size);
	
	UFUNCTION(BlueprintCallable)
	void OnDropCalled(UDragDropOperation* operation);

	UFUNCTION(BlueprintCallable)
	void OnRemoved();

	UFUNCTION(BlueprintCallable)
	void Refresh();

	UFUNCTION(BlueprintCallable)
	void OnDragDetectedCall();

	DelegateOnEquipmentRemoved DOnEquipmentRemoved;

	FORCEINLINE void SetItem(TWeakObjectPtr<UItemObject> newItemObj) { itemObj = newItemObj; }

	UFUNCTION(BlueprintCallable)
	UItemObject* GetItem() const;

	UFUNCTION(BlueprintCallable)
	FSlateBrush GetIconImage();

	UFUNCTION(BlueprintCallable)
	FSlateBrush GetOriginIconImage();

	void SetEquipmentAndBoxType(const EEquipmentBox newBoxType);

	void SetNumber(const int num);
	
	void SetDragVisualClass(TSubclassOf<UWidgetDragVisual> dragVisClass);

private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	USizeBox* BackGroundSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UBorder* BackGroundBorder;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetDragVisual* dragVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidgetDragVisual> dragVisualClass;

	UPROPERTY()
	UInventoryComponent* inventoryComponent;

	TWeakObjectPtr<UItemObject> itemObj;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	float widthSize = 120;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	float heightSize = 70;

	float tileSize;

	FVector2D widgetSize;

	UPROPERTY(BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true", BindWidget))
	EItemMainType equipmentType;

	EEquipmentBox boxType;
	
	int number;

};

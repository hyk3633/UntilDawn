// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetItemInventory.generated.h"

/**
 * 
 */

class UWidgetEquipmentWindow;
class UInventoryComponent;
class UWidgetInventoryGrid;

UCLASS()
class UNTILDAWN_API UWidgetItemInventory : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION(BlueprintCallable)
	void ItemDrop(UDragDropOperation* operation);

	FORCEINLINE void SetTileSize(float size) { tileSize = size; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetInvComp(UInventoryComponent* comp) { inventoryComponent = comp; }

private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetEquipmentWindow* EquipmentWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetInventoryGrid* InventoryGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* inventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	float tileSize = 70.f;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/Tile.h"
#include "WidgetItemGrid.generated.h"

/**
 * 
 */

class USizeBox;
class UBorder;
class UImage;
class UTextBlock;
class UDragDropOperation;
struct FSlateBrush;
class UItemObject;

DECLARE_DELEGATE_OneParam(DelegateOnRemoved, UItemObject* removedItem);

UCLASS()
class UNTILDAWN_API UWidgetItemGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void OnRemoved();

	UFUNCTION(BlueprintCallable)
	void Refresh();

	UFUNCTION(BlueprintCallable)
	void OnDragDetectedCall();

	DelegateOnRemoved DOnRemoved;

	void SetItem(TWeakObjectPtr<UItemObject> newItemObj);

	FORCEINLINE void SetTileSize(const float size) { tileSize = size; }

	UFUNCTION(BlueprintCallable)
	UItemObject* GetItem() const;

	UFUNCTION(BlueprintCallable)
	FSlateBrush GetIconImage();

	UFUNCTION()
	void UpdateItemQuantityText(const uint8 quantity);

private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	USizeBox* BackGroundSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UBorder* BackGroundBorder;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* ItemQuantity;

	TWeakObjectPtr<UItemObject> itemObj;

	float tileSize;

	FVector2D widgetSize;

};

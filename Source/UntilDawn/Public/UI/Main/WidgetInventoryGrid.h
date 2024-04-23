// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/Line.h"
#include "WidgetInventoryGrid.generated.h"

/**
 * 
 */

class UInventoryComponent;
class UItemObject;
class UWidgetItemGrid;
class UBorder;
class UCanvasPanel;
class UDragDropOperation;
class USlateBrushAsset;

UCLASS()
class UNTILDAWN_API UWidgetInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void InitializeWidget(UInventoryComponent* invComp, float size);

	FORCEINLINE FIntPoint GetDragged() const { return draggedItemTopLeftTile; }

	FORCEINLINE bool GetIsCursorInArea() const { return isCursorInArea; }

protected:

	void CreateLineSegments();

	UFUNCTION()
	void Refresh();

	UFUNCTION()
	void OnItemRemoved(UItemObject* removedItem);

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailableForPayload(UItemObject* payloadItem) const;

	UFUNCTION(BlueprintCallable)
	bool MousePositionOfTileRightLeft(float mousePositionX);

	UFUNCTION(BlueprintCallable)
	bool MousePositionOfTileUpDown(float mousePositionY);

	UFUNCTION(BlueprintPure)
	bool OnPaintCalled(FPaintContext context) const;

	UFUNCTION(BlueprintCallable)
	void OnDropCalled(UDragDropOperation* operation);

	UFUNCTION(BlueprintCallable)
	bool OnDragOverCalled(FPointerEvent pointEvent, FGeometry geometry, UDragDropOperation* operation);

	UFUNCTION(BlueprintCallable)
	void OnPreviewKeyDownCalled(const bool isKeyRight);

	UFUNCTION(BlueprintCallable)
	UItemObject* GetPayload(UDragDropOperation* operation) const;

	UFUNCTION(BlueprintCallable)
	void SetCursorInArea(const bool bIn);

private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UBorder* GridBorder;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UCanvasPanel* GridCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* inventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidgetItemGrid> itemWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	float tileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	FIntPoint draggedItemTopLeftTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	bool isCursorInArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TArray<FLine> lines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	USlateBrushAsset* slateBrush;

};

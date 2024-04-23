// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetDragVisual.generated.h"

/**
 * 
 */

class USizeBox;
class UBorder;
class UImage;
struct FSlateBrush;

UCLASS()
class UNTILDAWN_API UWidgetDragVisual : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void InitilizeWidget(const FVector2D& widgetSize, const FSlateBrush& brush);

private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	USizeBox* BackGroundSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UBorder* BackGroundBorder;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* ItemImage;
};

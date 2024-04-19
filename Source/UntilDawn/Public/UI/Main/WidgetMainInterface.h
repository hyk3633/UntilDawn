// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMainInterface.generated.h"

/**
 * 
 */

class UWidgetItemInventory;

UCLASS()
class UNTILDAWN_API UWidgetMainInterface : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeWidget();

	void ToggleInventoryUI();

private:
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UWidgetItemInventory* ItemInventory;

};

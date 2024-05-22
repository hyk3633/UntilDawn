// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetZombieHealth.generated.h"

/**
 * 
 */

class UProgressBar;

UCLASS()
class UNTILDAWN_API UWidgetZombieHealth : public UUserWidget
{
	GENERATED_BODY()

public:


private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

};

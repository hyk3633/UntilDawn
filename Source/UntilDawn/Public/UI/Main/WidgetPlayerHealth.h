// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetPlayerHealth.generated.h"

/**
 * 
 */

class UTextBlock;
class UProgressBar;
class APlayerControllerMainMap;

UCLASS()
class UNTILDAWN_API UWidgetPlayerHealth : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void InitHealthWidget(const FString& playerID, const float healthPercentage, TWeakObjectPtr<APlayerControllerMainMap> controller);

protected:

	UFUNCTION()
	void SetProgressPercentage(const float percentage);
	
private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerIDTextBlock;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

};

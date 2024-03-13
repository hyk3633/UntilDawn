// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetWrestlingProgress.generated.h"

/**
 * 
 */

class UProgressBar;
class UWidgetAnimation;

UCLASS()
class UNTILDAWN_API UWidgetWrestlingProgress : public UUserWidget
{
	GENERATED_BODY()
	
public:

	bool ReducingProgressBar(float deltaTime);

	bool IncreasingProgressBar();

	void ResetProgressBar();

	void PlayHighlightTextAnimation();

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ResistProgressBar;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HighlightText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ButtonAction;
};

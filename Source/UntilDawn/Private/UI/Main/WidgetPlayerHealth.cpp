// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetPlayerHealth.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWidgetPlayerHealth::InitHealthWidget(const FString& playerID, const float healthPercentage, TWeakObjectPtr<APlayerControllerMainMap> controller)
{
	PlayerIDTextBlock->SetText(FText::FromString(playerID));
	HealthProgressBar->SetPercent(healthPercentage);
	controller->DHealthChanged.BindUFunction(this, FName("SetProgressPercentage"));
}

void UWidgetPlayerHealth::SetProgressPercentage(const float percentage)
{
	HealthProgressBar->SetPercent(percentage);
}

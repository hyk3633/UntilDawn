// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/WidgetZombieHealth.h"
#include "Components/ProgressBar.h"
#include "Zombie/ZombieCharacter.h"

void UWidgetZombieHealth::InitHealthWidget(TWeakObjectPtr<AZombieCharacter> zombieCharacter)
{
	zombieCharacter->onHealthChanged.BindUFunction(this, FName("SetProgressPercentage"));
}

void UWidgetZombieHealth::InitializeProgressBar()
{
	HealthProgressBar->SetPercent(1.f);
}

void UWidgetZombieHealth::SetProgressPercentage(const float percentage)
{
	HealthProgressBar->SetPercent(percentage);
}

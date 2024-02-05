// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Login/PlayerControllerLoginMap.h"

APlayerControllerLoginMap::APlayerControllerLoginMap()
{
}

void APlayerControllerLoginMap::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly uiInputMode;
	SetInputMode(uiInputMode);
}

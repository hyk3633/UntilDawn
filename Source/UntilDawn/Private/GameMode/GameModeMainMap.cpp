// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeMainMap.h"
#include "Player/Main/PlayerControllerMainMap.h"

AGameModeMainMap::AGameModeMainMap()
{
	PlayerControllerClass = APlayerControllerMainMap::StaticClass();
}

void AGameModeMainMap::BeginPlay()
{
	Super::BeginPlay();
}

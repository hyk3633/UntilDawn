// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeLoginMap.h"
#include "Player/Login/PlayerControllerLoginMap.h"
#include "UI/Login/HUDLoginMap.h"

AGameModeLoginMap::AGameModeLoginMap()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = APlayerControllerLoginMap::StaticClass();
	HUDClass = AHUDLoginMap::StaticClass();
}

void AGameModeLoginMap::BeginPlay()
{
	Super::BeginPlay();

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Main/PlayerControllerMainMap.h"
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Player/PlayerCharacter.h"

APlayerControllerMainMap::APlayerControllerMainMap()
{
}

void APlayerControllerMainMap::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly gameInputMode;
	SetInputMode(gameInputMode);
}

void APlayerControllerMainMap::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	myCharacter = Cast<APlayerCharacter>(pawn);
	// 서버에 게임 맵에 접속했음을 알림
	UpdatePlayerInfo();
	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	clientSocket->NotifyAccessingGame(myInfo);
	GetWorldTimerManager().SetTimer(SynchronizeTimer, this, &APlayerControllerMainMap::SynchronizePlayerInfo, 0.016f, true);
}

void APlayerControllerMainMap::UpdatePlayerInfo()
{
	FVector&& location = std::move(GetPawn()->GetActorLocation());
	FVector&& velocity = std::move(GetPawn()->GetVelocity());
	FRotator&& rotation = std::move(GetPawn()->GetActorRotation());

	myInfo.vectorX = location.X;
	myInfo.vectorY = location.Y;
	myInfo.vectorZ = location.Z;
	myInfo.velocityX = velocity.X;
	myInfo.velocityY = velocity.Y;
	myInfo.velocityZ = velocity.Z;
	myInfo.pitch = rotation.Pitch;
	myInfo.yaw = rotation.Yaw;
	myInfo.roll = rotation.Roll;
}

void APlayerControllerMainMap::SynchronizePlayerInfo()
{
	UpdatePlayerInfo();
	clientSocket->SynchronizeMyCharacterInfo(myInfo);
}

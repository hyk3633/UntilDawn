// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Main/PlayerControllerMainMap.h"
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Player/PlayerCharacter.h"
#include "Zombie/ZombieCharacter.h"

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
	// ������ ���� �ʿ� ���������� �˸�
	myCharacter->UpdatePlayerInfo();
	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	clientSocket->NotifyAccessingGame(myCharacter->GetPlayerInfo().characterInfo);
	GetWorldTimerManager().SetTimer(SynchronizeTimer, this, &APlayerControllerMainMap::SynchronizePlayerInfo, 0.016f, true);
}

void APlayerControllerMainMap::SendPlayerInputAction(const EPlayerInputs inputType)
{
	clientSocket->SendPlayerInputAction(static_cast<int>(inputType));
}

void APlayerControllerMainMap::SynchronizePlayerInfo()
{
	myCharacter->UpdatePlayerInfo();
	clientSocket->SynchronizeMyCharacterInfo(myCharacter->GetPlayerInfo());
}

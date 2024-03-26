// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Main/PlayerControllerMainMap.h"
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Player/PlayerCharacter.h"
#include "Zombie/ZombieCharacter.h"
#include "Kismet/GameplayStatics.h"

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
	myCharacter->UpdatePlayerInfo();
	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	clientSocket->NotifyAccessingGame(myCharacter->GetPlayerInfo().characterInfo);
	GetWorldTimerManager().SetTimer(SynchronizeTimer, this, &APlayerControllerMainMap::SynchronizePlayerInfo, 0.016f, true);
}

void APlayerControllerMainMap::SendPlayerInputAction(const EPlayerInputs inputType)
{
	clientSocket->SendPlayerInputAction(static_cast<int>(inputType));
}

void APlayerControllerMainMap::SendPlayerBlockingResult(const bool isSuccessToBlocking)
{
	clientSocket->SendPlayerBlockingResult(isSuccessToBlocking);
}

void APlayerControllerMainMap::SendPickedItemInfo(const int itemNumber)
{
	clientSocket->SendPickedItemInfo(itemNumber);
}

void APlayerControllerMainMap::SendHitPlayerInfo(const int playerNumber)
{
	clientSocket->SendHitPlayerInfo(playerNumber);
}

void APlayerControllerMainMap::SendHitZombieInfo(const int zombieNumber)
{
	clientSocket->SendHitZombieInfo(zombieNumber);
}

void APlayerControllerMainMap::OutToLobby()
{
	GetWorldTimerManager().SetTimer(outToLobbyTimer, this, &APlayerControllerMainMap::OutToLobbyAfterDelay, 3.f);
	myCharacter->PlayerDead();
}

void APlayerControllerMainMap::SynchronizePlayerInfo()
{
	myCharacter->UpdatePlayerInfo();
	clientSocket->SynchronizeMyCharacterInfo(myCharacter->GetPlayerInfo());
	myCharacter->ResetPlayerInfoBitMask();
}

void APlayerControllerMainMap::OutToLobbyAfterDelay()
{
	UGameplayStatics::OpenLevel(this, TEXT("LoginMap"));
}

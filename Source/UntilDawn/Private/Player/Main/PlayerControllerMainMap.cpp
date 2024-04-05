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
	myCharacter->DZombieInRange.BindUFunction(this, FName("SendInRangeZombie"));
	myCharacter->DZombieOutRange.BindUFunction(this, FName("SendOutRangeZombie"));
	myCharacter->DZombieHitsMe.BindUFunction(this, FName("SendZombieHitsMe"));
	myCharacter->UpdatePlayerInfo();
	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	clientSocket->NotifyAccessingGame(myCharacter->GetPlayerInfo().characterInfo);
	GetWorldTimerManager().SetTimer(SynchronizeTimer, this, &APlayerControllerMainMap::SynchronizePlayerInfo, 0.2f, true);
}

void APlayerControllerMainMap::SendPlayerInputAction(const EPlayerInputs inputType)
{
	clientSocket->SendPlayerInputAction(static_cast<int>(inputType));
}

void APlayerControllerMainMap::SendInRangeZombie(int zombieNumber)
{
	clientSocket->SendInRangeZombie(zombieNumber);
}

void APlayerControllerMainMap::SendOutRangeZombie(int zombieNumber)
{
	clientSocket->SendOutRangeZombie(zombieNumber);
}

void APlayerControllerMainMap::SendZombieHitsMe(int zombieNumber, bool bResult)
{
	clientSocket->SendZombieHitsMe(zombieNumber, bResult);
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

void APlayerControllerMainMap::PlayerDead()
{
	myCharacter->PlayerDead();
	GetWorldTimerManager().SetTimer(respawnRequestTimer, this, &APlayerControllerMainMap::respawnRequestAfterDelay, 3.f);
	DPlayerDead.ExecuteIfBound();
}

void APlayerControllerMainMap::SynchronizePlayerInfo()
{
	myCharacter->UpdatePlayerInfo();
	clientSocket->SynchronizeMyCharacterInfo(myCharacter->GetPlayerInfo());
}

void APlayerControllerMainMap::respawnRequestAfterDelay()
{
	clientSocket->SendRespawnRequest();
}

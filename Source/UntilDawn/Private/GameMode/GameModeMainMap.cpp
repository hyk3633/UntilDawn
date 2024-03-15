// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeMainMap.h"
#include "GameSystem/ActorSpawner.h"
#include "GameSystem/ZombieActorPooler.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"
#include "UI/Main/HUDMainMap.h"
#include "Zombie/ZombieCharacter.h"
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/ZombieState.h"

AGameModeMainMap::AGameModeMainMap()
{
	PrimaryActorTick.bCanEverTick = true;

	actorSpawner = CreateDefaultSubobject<UActorSpawner>(TEXT("Actor Spawner"));

	zombiePooler = CreateDefaultSubobject<UZombieActorPooler>(TEXT("Zombie Pooler"));

	PlayerControllerClass = APlayerControllerMainMap::StaticClass();
	DefaultPawnClass = nullptr;
	HUDClass = AHUDMainMap::StaticClass();
}

void AGameModeMainMap::BeginPlay()
{
	Super::BeginPlay();

	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	clientSocket->SetGameMode(this);

	myNumber = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetPlayerNumber();

	GetWorldTimerManager().SetTimer(playerSpawnDelayTimer, this, &AGameModeMainMap::PlayerSpawnAfterDelay, 0.5f);

	// 좀비 캐릭터 스폰 및 풀링
	zombiePooler->SetPoolSize(3);
	actorSpawner->SpawnActor(zombiePooler->GetPoolSize(), zombiePooler->GetActorPool());
}

void AGameModeMainMap::PlayerSpawnAfterDelay()
{
	// 내 클라이언트 캐릭터 스폰 및 컨트롤러 할당
	APlayerCharacter* myPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator);
	APlayerController* myPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	myPlayerController->Possess(myPlayerCharacter);
	playerCharacterMap.Add(myNumber, myPlayerCharacter);
}

void AGameModeMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!playerToDelete.IsEmpty())
	{
		DestroyPlayer();
	}
	if (playerInfoSetEx)
	{
		SpawnNewPlayerCharacter();
	}
	if (playerInfoSet)
	{
		SynchronizePlayersInfo();
	}
	if (zombieInfoSet)
	{
		SynchronizeZombieInfo();
	}
}

void AGameModeMainMap::DestroyPlayer()
{
	int number;
	while (!playerToDelete.IsEmpty())
	{
		playerToDelete.Dequeue(number);
		APlayerCharacter* tempCharacter = playerCharacterMap[number];
		tempCharacter->Destroy();
		playerCharacterMap.Remove(number);
	}
}

void AGameModeMainMap::ReceiveNewPlayerInfo(PlayerInfoSetEx* newPlayerInfoSet)
{
	playerInfoSetEx = newPlayerInfoSet;
}

void AGameModeMainMap::ReceiveOtherPlayersInfo(PlayerInfoSet* synchPlayerInfoSet)
{
	playerInfoSet = synchPlayerInfoSet;
}

void AGameModeMainMap::ReceiveZombieInfo(ZombieInfoSet* synchZombieInfoSet)
{
	zombieInfoSet = synchZombieInfoSet;
}

void AGameModeMainMap::ReceiveDisconnectedPlayerInfo(const int playerNumber, const FString playerID)
{
	if (playerCharacterMap.Find(playerNumber))
	{
		APlayerCharacter* tempCharacter = playerCharacterMap[playerNumber];
		if (IsValid(tempCharacter) && tempCharacter->GetPlayerID().Compare(playerID) == 0)
		{
			playerToDelete.Enqueue(playerNumber);
		}
	}
}

void AGameModeMainMap::SynchronizeOtherPlayerInputAction(const int playerNumber, const int inputType)
{
	if (playerCharacterMap.Find(playerNumber))
	{
		APlayerCharacter* tempCharacter = playerCharacterMap[playerNumber];
		if (IsValid(tempCharacter))
		{
			tempCharacter->DoPlayerInputAction(inputType);
		}
	}
}

void AGameModeMainMap::SpawnNewPlayerCharacter()
{
	for (auto& kv : playerInfoSetEx->characterInfoMap)
	{
		int number = kv.first;
		if (number == myNumber) continue;
		CharacterInfo& info = kv.second.characterInfo;
		APlayerCharacter* newPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>
			(
				APlayerCharacter::StaticClass(),
				FVector(info.vectorX, info.vectorY, info.vectorZ),
				FRotator(info.pitch, info.yaw, info.roll)
			);
		newPlayerCharacter->SetPlayerID(FString(UTF8_TO_TCHAR(playerInfoSetEx->playerIDMap[number].c_str())));
		newPlayerCharacter->SpawnDefaultController();
		playerCharacterMap.Add(number, newPlayerCharacter);
	}
	playerInfoSetEx = nullptr;
}

void AGameModeMainMap::SynchronizePlayersInfo()
{
	for (auto& playerInfo : playerInfoSet->characterInfoMap)
	{
		const int bitMax = static_cast<int>(PIBTS::MAX);
		for (int bit = 0; bit < bitMax; bit++)
		{
			if (playerInfo.second.recvInfoBitMask & (1 << bit))
			{
				ProcessPlayerInfo(playerInfo.first, playerInfo.second, bit);
			}
		}
		if (playerInfo.first != myNumber && playerCharacterMap.Find(playerInfo.first))
		{
			APlayerCharacter* character = playerCharacterMap[playerInfo.first];
			CharacterInfo& info = playerInfo.second.characterInfo;
			if (IsValid(character))
			{
				character->AddMovementInput(FVector(info.velocityX, info.velocityY, info.velocityZ));
				character->SetActorRotation(FRotator(info.pitch, info.yaw, info.roll));
				character->SetActorLocation(FVector(info.vectorX, info.vectorY, info.vectorZ));
			}
		}
	}
}

void AGameModeMainMap::ProcessPlayerInfo(const int playerNumber, const PlayerInfo& info, const int bitType)
{
	PIBTS type = static_cast<PIBTS>(bitType);
	switch (type)
	{
		case PIBTS::WrestlingState:
		{
			if (info.wrestleState == EWrestleState::WRESTLING)
			{
				playerCharacterMap[playerNumber]->SetWrestlingOn();
			}
			break;
		}
		case PIBTS::PlayGrabReaction:
		{
			APlayerCharacter* character = playerCharacterMap[playerNumber];
			character->PlayPushingZombieMontage(info.isBlockingAction);
			playerCharacterMap[playerNumber]->SetWrestlingOff();
			break;
		}
	}
}

void AGameModeMainMap::SynchronizeZombieInfo()
{
	for (auto& info : zombieInfoSet->zombieInfoMap)
	{
		const ZombieInfo& zombieInfo = info.second;
		AZombieCharacter* newZombie;

		if (zombieCharacterMap.Find(info.first) == nullptr)
		{
			newZombie = zombiePooler->GetPooledActor();
			if (newZombie == nullptr)
			{
				actorSpawner->SpawnActor(1, zombiePooler->GetActorPool());
				newZombie = zombiePooler->GetPooledActor();
			}
			newZombie->SetNumber(info.first);
			newZombie->SetActorRotation(zombieInfo.rotation);
			newZombie->ActivateActor();
			zombieCharacterMap.Add(info.first, newZombie);
		}
		else
		{
			newZombie = zombieCharacterMap[info.first];
		}

		if (info.second.targetNumber >= 0 && playerCharacterMap.Find(info.second.targetNumber))
		{
			newZombie->SetTarget(playerCharacterMap[info.second.targetNumber]);
		}

		if (info.second.state == EZombieState::GRAB)
		{
			newZombie->SetActorRotation(zombieInfo.rotation);
		}

		newZombie->SetNextLocation(zombieInfo.nextLocation);
		newZombie->SetActorLocation(zombieInfo.location);
		newZombie->SetZombieState(info.second.state);
	}
	zombieInfoSet = nullptr;
}

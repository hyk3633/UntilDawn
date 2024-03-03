// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeMainMap.h"
#include "GameSystem/ActorSpawner.h"
#include "GameSystem/ZombieActorPooler.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"
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
}

void AGameModeMainMap::BeginPlay()
{
	Super::BeginPlay();

	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	clientSocket->SetGameMode(this);

	myNumber = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetPlayerNumber();

	// 내 클라이언트 캐릭터 스폰 및 컨트롤러 할당
	APlayerCharacter* myPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator);
	APlayerController* myPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	myPlayerController->Possess(myPlayerCharacter);
	playerCharacterMap.Add(myNumber, myPlayerCharacter);

	// 좀비 캐릭터 스폰 및 풀링
	zombiePooler->SetPoolSize(3);
	actorSpawner->SpawnActor(zombiePooler->GetPoolSize(), zombiePooler->GetActorPool());
}

void AGameModeMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!playerToDelete.IsEmpty())
		DestroyPlayer();
	if (playerInfoSetEx)
		SpawnNewPlayerCharacter();
	if (playerInfoSet)
		SynchronizeOtherPlayersInfo();
	if (zombieInfoSet)
		SynchronizeZombieInfo();
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

void AGameModeMainMap::SynchronizeOtherPlayersInfo()
{
	for (auto& playerInfo : playerInfoSet->characterInfoMap)
	{
		if (playerInfo.first == myNumber) continue;
		CharacterInfo& info = playerInfo.second.characterInfo;
		if (playerCharacterMap.Find(playerInfo.first))
		{
			APlayerCharacter* character = playerCharacterMap[playerInfo.first];
			if (IsValid(character))
			{
				character->AddMovementInput(FVector(info.velocityX, info.velocityY, info.velocityZ));
				character->SetActorRotation(FRotator(info.pitch, info.yaw, info.roll));
				character->SetActorLocation(FVector(info.vectorX, info.vectorY, info.vectorZ));
			}
		}
	}
}

void AGameModeMainMap::SynchronizeZombieInfo()
{
	for (auto& info : zombieInfoSet->zombieInfoMap)
	{
		const CharacterInfo& chaInfo = info.second.characterInfo;
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
			newZombie->ActivateActor();
			zombieCharacterMap.Add(info.first, newZombie);
			newZombie->SetActorLocation(FVector(chaInfo.vectorX, chaInfo.vectorY, chaInfo.vectorZ));
		}
		else
		{
			newZombie = zombieCharacterMap[info.first];
		}
		//const CharacterInfo& chaInfo = info.second.characterInfo;
		newZombie->AddMovementInput(FVector(chaInfo.velocityX, chaInfo.velocityY, chaInfo.velocityZ));
		newZombie->SetActorLocation(FVector(chaInfo.vectorX, chaInfo.vectorY, chaInfo.vectorZ));
		newZombie->SetZombieState(info.second.state);
		//newZombie->SetTargetLocation(FVector(info.second.x, info.second.y, info.second.z));
		//newZombie->Move();
		// velocity
		// rotation
	}
	zombieInfoSet = nullptr;
}

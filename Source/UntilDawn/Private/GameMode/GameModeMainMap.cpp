// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeMainMap.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Kismet/GameplayStatics.h"

AGameModeMainMap::AGameModeMainMap()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<APlayerCharacter> PlayerPawnBPClass(TEXT("/Game/_Assets/Blueprints/Player/BP_PlayerCharacter"));
	defaultPawnClass = PlayerPawnBPClass.Class;

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
	APlayerCharacter* myPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(defaultPawnClass, FVector(0, 0, 0), FRotator::ZeroRotator);
	APlayerController* myPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	myPlayerController->Possess(myPlayerCharacter);
	playerCharacterMap.Add(myNumber, myPlayerCharacter);
}

void AGameModeMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (playerInfoSetEx)
		SpawnNewPlayerCharacter();
	if (playerInfoSet)
		SynchronizeOtherPlayersInfo();
}

void AGameModeMainMap::ReceiveNewPlayerInfo(PlayerInfoSetEx* newPlayerInfoSet)
{
	playerInfoSetEx = newPlayerInfoSet;
}

void AGameModeMainMap::ReceiveOtherPlayersInfo(PlayerInfoSet* synchPlayerInfoSet)
{
	playerInfoSet = synchPlayerInfoSet;
}

void AGameModeMainMap::SpawnNewPlayerCharacter()
{
	for (auto& playerInfo : playerInfoSetEx->playerInfoMap)
	{
		int number = playerInfo.first;
		if (number == myNumber) continue;
		PlayerInfo& info = playerInfo.second;
		APlayerCharacter* newPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>
			(
				defaultPawnClass,
				FVector(info.vectorX, info.vectorY, info.vectorZ),
				FRotator(info.pitch, info.yaw, info.roll)
			);
		newPlayerCharacter->SetPlayerId(FString(UTF8_TO_TCHAR(playerInfoSetEx->playerIDMap[number].c_str())));
		newPlayerCharacter->SpawnDefaultController();
		playerCharacterMap.Add(number, newPlayerCharacter);
	}
	playerInfoSetEx = nullptr;
}

void AGameModeMainMap::SynchronizeOtherPlayersInfo()
{
	for (auto& playerInfo : playerInfoSet->playerInfoMap)
	{
		if (playerInfo.first == myNumber) continue;
		PlayerInfo& info = playerInfo.second;
		if (playerCharacterMap.Find(playerInfo.first))
		{
			APlayerCharacter* character = playerCharacterMap[playerInfo.first];
			character->AddMovementInput(FVector(info.velocityX, info.velocityY, info.velocityZ));
			character->SetActorRotation(FRotator(info.pitch, info.yaw, info.roll));
			character->SetActorLocation(FVector(info.vectorX, info.vectorY, info.vectorZ));
		}
	}
}

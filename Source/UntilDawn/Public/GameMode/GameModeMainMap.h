// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structs/CharacterInfo.h"
#include "GameModeMainMap.generated.h"

/**
 * 
 */

class UActorSpawner;
class UZombieActorPooler;
class ClientSocket;
class APlayerCharacter;
class AZombieCharacter;
class PlayerInfoSetEx;

UCLASS()
class UNTILDAWN_API AGameModeMainMap : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGameModeMainMap();

protected:

	virtual void BeginPlay() override;

	void PlayerSpawnAfterDelay();

	void DestroyPlayer();

	void SpawnNewPlayerCharacter();

	void SynchronizePlayersInfo();

	void ProcessPlayerInfo(const int playerNumber, const PlayerInfo& info, const int bitType);

	void SynchronizeZombieInfo();

	void ProcessZombieInfo(AZombieCharacter* zombie, const ZombieInfo& info, const int bitType);

public:

	virtual void Tick(float deltaTime) override;

	void ReceiveNewPlayerInfo(PlayerInfoSetEx* newPlayerInfoSet);

	void ReceiveOtherPlayersInfo(PlayerInfoSet* synchPlayerInfoSet);

	void ReceiveDisconnectedPlayerInfo(const int playerNumber, const FString playerID);

	void SynchronizeOtherPlayerInputAction(const int playerNumber, const int inputType);

	void ReceiveZombieInfo(ZombieInfoSet* synchZombieInfoSet);

private:

	UPROPERTY()
	UActorSpawner* actorSpawner;

	UPROPERTY()
	UZombieActorPooler* zombiePooler;
	
	ClientSocket* clientSocket;

	TQueue<int> playerToDelete;

	TMap<int, APlayerCharacter*> playerCharacterMap;

	PlayerInfoSetEx* playerInfoSetEx;

	PlayerInfoSet* playerInfoSet;

	TMap<int, AZombieCharacter*> zombieCharacterMap;

	ZombieInfoSet* zombieInfoSet;

	int myNumber;

	FTimerHandle playerSpawnDelayTimer;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structs/CharacterInfo.h"
#include <sstream>
#include "GameModeMainMap.generated.h"

/**
 * 
 */

class UActorSpawner;
class UActorPooler;
class ClientSocket;
class APlayerCharacter;
class AZombieCharacter;
class PlayerInfoSetEx;
class ItemInfoSet;
class AItemBase;

UCLASS()
class UNTILDAWN_API AGameModeMainMap : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGameModeMainMap();

protected:

	virtual void BeginPlay() override;

	void ProcessPacket();

	void SpawnNewPlayerCharacter(std::stringstream& recvStream);

	void SynchronizePlayers(std::stringstream& recvStream);

	void SynchronizeZombies(std::stringstream& recvStream);

	void SynchronizeItems(std::stringstream& recvStream);

	void InitializeWorld(std::stringstream& recvStream);

	void SynchronizeOtherPlayerInputAction(std::stringstream& recvStream);

	void PlayWrestlingResultAction(std::stringstream& recvStream);

	void StartPlayerWrestling(std::stringstream& recvStream);

	void DestroyItem(std::stringstream& recvStream);

	void PickUpItem(std::stringstream& recvStream);

	void ProcessDisconnectedPlayer(std::stringstream& recvStream);

	void ProcessPlayerDead(std::stringstream& recvStream);

	void RespawnPlayer(std::stringstream& recvStream);

	void ProcessZombieDead(std::stringstream& recvStream);

	void PlayerSpawnAfterDelay();

public:

	virtual void Tick(float deltaTime) override;

private:

	UPROPERTY()
	UActorSpawner* actorSpawner;

	UPROPERTY()
	UActorPooler* zombiePooler;

	UPROPERTY()
	UActorPooler* itemPooler;
	
	ClientSocket* clientSocket;

	TMap<int, APlayerCharacter*> playerCharacterMap;

	TMap<int, AZombieCharacter*> zombieCharacterMap;

	TMap<int, AItemBase*> itemMap;

	int myNumber;

	FTimerHandle playerSpawnDelayTimer;

	std::vector<void (AGameModeMainMap::*)(std::stringstream&)> packetCallbacks;

};

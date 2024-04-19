// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structs/CharacterInfo.h"
#include "Enums/PacketType.h"
#include <unordered_map>
#include <sstream>
#include "GameModeMainMap.generated.h"

/**
 * 
 */

class UActorSpawner;
class UActorPooler;
class ClientSocket;
class UItemManager;
class UItemObject;
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

	void ProcessPacket();

	void SpawnNewPlayerCharacter(std::stringstream& recvStream);

	void SynchronizePlayers(std::stringstream& recvStream);

	void SynchronizeZombies(std::stringstream& recvStream);

	void SynchronizeItems(std::stringstream& recvStream);

	void InitializeWorld(std::stringstream& recvStream);

	void SynchronizeOtherPlayerInputAction(std::stringstream& recvStream);

	void PlayWrestlingResultAction(std::stringstream& recvStream);

	void StartPlayerWrestling(std::stringstream& recvStream);

	void ItemPickUpOtherPlayer(std::stringstream& recvStream);

	void ProcessDisconnectedPlayer(std::stringstream& recvStream);

	void ProcessPlayerDead(std::stringstream& recvStream);

	void RespawnPlayer(std::stringstream& recvStream);

	void ProcessZombieDead(std::stringstream& recvStream);

	void SpawnItems(std::stringstream& recvStream);

	void PlayerSpawnAfterDelay();

public:

	void DropItem(TWeakObjectPtr<UItemObject> droppedItemObj);

	virtual void Tick(float deltaTime) override;

private:

	UPROPERTY()
	UActorSpawner* actorSpawner;

	UPROPERTY()
	UActorPooler* zombiePooler;

	UPROPERTY()
	UItemManager* itemManager;
	
	ClientSocket* clientSocket;

	TMap<int, APlayerCharacter*> playerCharacterMap;

	TMap<int, TWeakObjectPtr<AZombieCharacter>> zombieCharacterMap;

	int myNumber;

	FTimerHandle playerSpawnDelayTimer;

	std::unordered_map<EPacketType, void (AGameModeMainMap::*)(std::stringstream&)> packetCallbacks;

};

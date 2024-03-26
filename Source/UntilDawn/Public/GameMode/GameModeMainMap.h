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

	void PlayerSpawnAfterDelay();

	void DestroyPlayer();

	void SpawnNewPlayerCharacter();

	void SynchronizePlayersInfo();

	void ProcessPlayerInfo(const int playerNumber, const PlayerInfo& info, const int bitType);

	void SynchronizeZombieInfo();

	void SynchronizeItemInfo();

	void ProcessWrestlingResult();

	void StartPlayerWrestlingAction();

	void DestroyItem();

	void PickUpItem();

	void ProcessZombieDead();

	void ProcessPlayerDead();

public:

	virtual void Tick(float deltaTime) override;

	void ReceiveNewPlayerInfo(PlayerInfoSetEx* newPlayerInfoSet);

	void ReceiveOtherPlayersInfo(PlayerInfoSet* synchPlayerInfoSet);

	void ReceiveDisconnectedPlayerInfo(const int playerNumber);

	void SynchronizeOtherPlayerInputAction(const int playerNumber, const int inputType);

	void PlayWrestlingResultAction(const int playerNumber, const bool wrestlingResult);

	void ReceiveWrestlingPlayer(const int playerNumber);

	void ReceiveZombieInfo(ZombieInfoSet* synchZombieInfoSet);

	void ReceiveItemInfo(ItemInfoSet* synchItemInfoSet);

	void DestroyItem(const int itemNumber);

	void PickUpItem(const int itemNumber);

	void ReceiveDeadZombieNumber(const int zombieNumber);

	void ReceiveDeadPlayerNumber(const int playerNumber);

private:

	UPROPERTY()
	UActorSpawner* actorSpawner;

	UPROPERTY()
	UActorPooler* zombiePooler;

	UPROPERTY()
	UActorPooler* itemPooler;
	
	ClientSocket* clientSocket;

	TQueue<int> playerToDelete;

	TMap<int, APlayerCharacter*> playerCharacterMap;

	PlayerInfoSetEx* playerInfoSetEx;

	PlayerInfoSet* playerInfoSet;

	TMap<int, AZombieCharacter*> zombieCharacterMap;

	ZombieInfoSet* zombieInfoSet;

	TMap<int, AItemBase*> itemMap;

	ItemInfoSet* itemInfoSet;

	int myNumber;

	FTimerHandle playerSpawnDelayTimer;

	TQueue<std::pair<int, bool>> wrestlingResultQ;

	TQueue<int> wrestlingStartQ;

	TQueue<int> destroyItemQ;

	TQueue<int> pickUpItemQ;

	TQueue<int> deadZombieQ;

	TQueue<int> deadPlayerQ;

};

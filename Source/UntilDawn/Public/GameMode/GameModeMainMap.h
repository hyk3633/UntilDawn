// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structs/CharacterInfo.h"
#include "Structs/ItemInfo.h"
#include "Structs/ItemAsset.h"
#include "Enums/PacketType.h"
#include <unordered_map>
#include <sstream>
#include "GameModeMainMap.generated.h"

/**
 * 
 */

class UJsonComponent;
class UActorSpawner;
class UActorPooler;
class ClientSocket;
class APlayerCharacter;
class AZombieCharacter;
class PlayerInfoSetEx;
class ItemInfoSet;
class AItemBase;
class ItemCore;

UCLASS()
class UNTILDAWN_API AGameModeMainMap : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGameModeMainMap();

protected:

	virtual void BeginPlay() override;

	void LoadItemInfoAndAsset();

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

	void SpawnItems(std::stringstream& recvStream);

	void PlayerSpawnAfterDelay();

public:

	virtual void Tick(float deltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Item Info", meta = (AllowPrivateAccess = "true"))
	UJsonComponent* jsonComponent;

	UPROPERTY()
	UActorSpawner* actorSpawner;

	UPROPERTY()
	UActorPooler* zombiePooler;

	UPROPERTY()
	TMap<int, UActorPooler*> itemPoolerMap;

	TMap<int, FItemInfo*> itemInfoMap;

	TMap<int, TSharedPtr<FItemAsset>> itemAssetMap;
	
	ClientSocket* clientSocket;

	TMap<int, APlayerCharacter*> playerCharacterMap;

	TMap<int, TWeakObjectPtr<AZombieCharacter>> zombieCharacterMap;

	TMap<int, TWeakObjectPtr<AItemBase>> itemMap;

	int myNumber;

	FTimerHandle playerSpawnDelayTimer;

	std::unordered_map<EPacketType, void (AGameModeMainMap::*)(std::stringstream&)> packetCallbacks;

};

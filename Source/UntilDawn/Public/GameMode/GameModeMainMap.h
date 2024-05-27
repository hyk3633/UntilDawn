// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structs/CharacterInfo.h"
#include "Structs/Tile.h"
#include "Structs/PossessedItem.h"
#include "Structs/EquippedItem.h"
#include "Structs/PlayerStatus.h"
#include "Structs/HitInfo.h"
#include "Enums/PacketType.h"
#include <unordered_map>
#include <sstream>
#include <string>
#include "GameModeMainMap.generated.h"

/**
 * 
 */

class UActorPooler;
class ClientSocket;
class UItemManager;
class UItemObject;
class AItemBase;
class APlayerControllerMainMap;
class APlayerCharacter;
class AZombieCharacter;
class PlayerInfoSetEx;
class AProjectileBase;

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

	void PlayerItemPickUp(std::stringstream& recvStream);

	void PlayerItemGridPointUpdate(std::stringstream& recvStream);

	void PlayerItemEquip(std::stringstream& recvStream);

	void PlayerUnequipItem(std::stringstream& recvStream);

	void PlayerItemDrop(std::stringstream& recvStream);

	void PlayerDropEquippedItem(std::stringstream& recvStream);

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

	void InitializePlayerPossessedItems(std::stringstream& recvStream);

	void InitializePlayerEquippedItems(std::stringstream& recvStream);

	void PlayerSpawnAfterDelay();

	void ReceiveReplicatedProjectile(std::stringstream& recvStream);

	void PlayerUseItem(std::stringstream& recvStream);

	void UpdateCharacterHealth(std::stringstream& recvStream);

	void InitializePlayerInitialInfo(std::stringstream& recvStream);

	void PlayerChangeWeapon(std::stringstream& recvStream);

	void PlayerArmWeapon(std::stringstream& recvStream);

	void PlayerDisarmWeapon(std::stringstream& recvStream);

	void ProcessAttackResult(std::stringstream& recvStream);

public:

	void DropItem(TWeakObjectPtr<APlayerCharacter> dropper, TWeakObjectPtr<AItemBase> droppedItem);

	TWeakObjectPtr<AProjectileBase> GetProjectile() const;

	virtual void Tick(float deltaTime) override;

private:

	UPROPERTY()
	UActorPooler* zombiePooler;

	UPROPERTY()
	UActorPooler* projectilePooler;

	UPROPERTY()
	UItemManager* itemManager;
	
	ClientSocket* clientSocket;
	
	TMap<int, APlayerCharacter*> playerCharacterMap;

	TMap<int, TWeakObjectPtr<AZombieCharacter>> zombieCharacterMap;

	FString myID;

	int myNumber;

	FTimerHandle playerSpawnDelayTimer;

	std::unordered_map<EPacketType, void (AGameModeMainMap::*)(std::stringstream&)> packetCallbacks;

	TWeakObjectPtr<APlayerControllerMainMap> myController;

	bool bProcessPacket;

};

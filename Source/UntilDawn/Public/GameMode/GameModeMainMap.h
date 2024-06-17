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
#include "Structs/ZombieAsset.h"
#include "Structs/ItemFieldRotation.h"
#include "Enums/PacketType.h"
#include "Enums/InputType.h"
#include "Tag/UntilDawnGameplayTags.h"
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
class UDataTable;

UCLASS()
class UNTILDAWN_API AGameModeMainMap : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGameModeMainMap();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float deltaTime) override;

protected:

	/* 패킷 처리 */

	// 소켓의 메시지 큐에서 데이터를 꺼내옴
	void ProcessPacket();

	// 월드 정보 초기화
	void InitializeWorld(std::stringstream& recvStream);

	/* 플레이어 정보 및 상태 동기화 */

	void SpawnNewPlayerCharacter(std::stringstream& recvStream);

	void InitializePlayerInitialInfo(std::stringstream& recvStream);

	void InitializePlayerPossessedItems(std::stringstream& recvStream);

	void InitializePlayerEquippedItems(std::stringstream& recvStream);

	void SynchronizePlayers(std::stringstream& recvStream);

	void UpdateCharacterHealth(std::stringstream& recvStream);

	void ProcessPlayerDead(std::stringstream& recvStream);

	void RespawnPlayer(std::stringstream& recvStream);

	void ProcessDisconnectedPlayer(std::stringstream& recvStream);

	/* 플레이어 좀비 레슬링 */

	void StartWrestling(std::stringstream& recvStream);

	void PlayWrestlingResultAction(std::stringstream& recvStream);

	void CancelWrestling(std::stringstream& recvStream);

	/* 인벤토리의 아이템 그리드 위치 변경 */

	void UpdateInventoryItemGridPoint(std::stringstream& recvStream);

	/* 플레이어 공격 동기화 */

	void ActivateWeaponAbility(std::stringstream& recvStream);

	void ProcessAttackResult(std::stringstream& recvStream);

	void ProcessKickedCharacters(std::stringstream& recvStream);

	void ReplicateProjectile(std::stringstream& recvStream);

	/* 아이템 동기화 */

	void SpawnItems(std::stringstream& recvStream);

	void PickUpItem(std::stringstream& recvStream);

	void DropItem(std::stringstream& recvStream);

	void EquipItem(std::stringstream& recvStream);

	void UnequipItem(std::stringstream& recvStream);

	void DropEquippedItem(std::stringstream& recvStream);

	void ArmWeapon(std::stringstream& recvStream);

	void DisarmWeapon(std::stringstream& recvStream);

	void ChangeWeapon(std::stringstream& recvStream);

	void UseItem(std::stringstream& recvStream);

	/* 좀비 동기화 */

	void SynchronizeZombies(std::stringstream& recvStream);

	void ProcessZombieHit(std::stringstream& recvStream);

	void ProcessZombieDead(std::stringstream& recvStream);

	USkeletalMesh* GetZombieMesh(const int32 zombieNumber);

	void SpawnPlayerAfterDelay();

public:

	void RespawnDroppedItem(TWeakObjectPtr<APlayerCharacter> dropper, TWeakObjectPtr<AItemBase> droppedItem);

	TWeakObjectPtr<AProjectileBase> GetProjectile() const;

protected:

	// 스폰할 아이템이 적절한 모습으로 보이도록 위치 조정
	void SetFieldItemTransform(TWeakObjectPtr<AItemBase> droppedItem, FVector spawnLocation);

	FTransform GetPlayerStartTransform();

private:

	UPROPERTY()
	UActorPooler* zombiePooler;

	UPROPERTY()
	UDataTable* zombieAssetDataTable;

	UPROPERTY()
	UDataTable* itemFieldRotationDataTable;

	UPROPERTY()
	UActorPooler* projectilePooler;

	UPROPERTY()
	UItemManager* itemManager;
	
	ClientSocket* clientSocket;

	UPROPERTY()
	TSubclassOf<APlayerCharacter> playerCharacterClass;
	
	TMap<int, APlayerCharacter*> playerCharacterMap;

	TMap<int, TWeakObjectPtr<AZombieCharacter>> zombieCharacterMap;

	FString myID;

	int myNumber;

	FTimerHandle playerSpawnDelayTimer;

	std::unordered_map<EPacketType, void (AGameModeMainMap::*)(std::stringstream&)> packetCallbacks;

	TWeakObjectPtr<APlayerControllerMainMap> myController;

	bool bProcessPacket;

};

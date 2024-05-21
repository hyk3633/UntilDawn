// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeMainMap.h"
#include "GameSystem/ItemManager.h"
#include "GameSystem/ActorPooler.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"
#include "UI/Main/HUDMainMap.h"
#include "Zombie/ZombieCharacter.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject.h"
#include "Item/Projectile/ProjectileBase.h"
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/ZombieState.h"
#include "Enums/PoolableActorType.h"
#include "Interface/PoolableActor.h"

using std::string;

AGameModeMainMap::AGameModeMainMap()
{
	PrimaryActorTick.bCanEverTick = true;

	itemManager = CreateDefaultSubobject<UItemManager>(TEXT("Item Manager"));

	zombiePooler = CreateDefaultSubobject<UActorPooler>(TEXT("Zombie Pooler"));
	zombiePooler->SetActorClass(AZombieCharacter::StaticClass());

	projectilePooler = CreateDefaultSubobject<UActorPooler>(TEXT("Projectile Pooler"));
	projectilePooler->SetActorClass(AProjectileBase::StaticClass());

	PlayerControllerClass = APlayerControllerMainMap::StaticClass();
	DefaultPawnClass = nullptr;
	HUDClass = AHUDMainMap::StaticClass();
}

void AGameModeMainMap::BeginPlay()
{
	Super::BeginPlay();
	
	packetCallbacks[EPacketType::SPAWNPLAYER]			= &AGameModeMainMap::SpawnNewPlayerCharacter;
	packetCallbacks[EPacketType::SYNCHPLAYER]			= &AGameModeMainMap::SynchronizePlayers;
	packetCallbacks[EPacketType::SYNCHZOMBIE]			= &AGameModeMainMap::SynchronizeZombies;
	packetCallbacks[EPacketType::PICKUP_ITEM]			= &AGameModeMainMap::PlayerItemPickUp;
	packetCallbacks[EPacketType::ITEMGRIDPOINTUPDATE]	= &AGameModeMainMap::PlayerItemGridPointUpdate;
	packetCallbacks[EPacketType::EQUIP_ITEM]			= &AGameModeMainMap::PlayerItemEquip;
	packetCallbacks[EPacketType::UNEQUIP_ITEM]			= &AGameModeMainMap::PlayerUnequipItem;
	packetCallbacks[EPacketType::DROP_ITEM]				= &AGameModeMainMap::PlayerItemDrop;
	packetCallbacks[EPacketType::DROP_EQUIPPED_ITEM]	= &AGameModeMainMap::PlayerDropEquippedItem;
	packetCallbacks[EPacketType::INITIALINFO]			= &AGameModeMainMap::InitializeWorld;
	packetCallbacks[EPacketType::PLAYERINPUTACTION]		= &AGameModeMainMap::SynchronizeOtherPlayerInputAction;
	packetCallbacks[EPacketType::WRESTLINGRESULT]		= &AGameModeMainMap::PlayWrestlingResultAction;
	packetCallbacks[EPacketType::WRESTLINGSTART]		= &AGameModeMainMap::StartPlayerWrestling;
	packetCallbacks[EPacketType::PLAYERDISCONNECTED]	= &AGameModeMainMap::ProcessDisconnectedPlayer;
	packetCallbacks[EPacketType::ITEMPICKUPOTHER]		= &AGameModeMainMap::ItemPickUpOtherPlayer;
	packetCallbacks[EPacketType::PLAYERDEAD]			= &AGameModeMainMap::ProcessPlayerDead;
	packetCallbacks[EPacketType::PLAYERRESPAWN]			= &AGameModeMainMap::RespawnPlayer;
	packetCallbacks[EPacketType::ZOMBIEDEAD]			= &AGameModeMainMap::ProcessZombieDead;
	packetCallbacks[EPacketType::SPAWNITEM]				= &AGameModeMainMap::SpawnItems;
	packetCallbacks[EPacketType::PLAYERINVENTORY]		= &AGameModeMainMap::InitializePlayerPossessedItems;
	packetCallbacks[EPacketType::PLAYEREQUIPMENT]		= &AGameModeMainMap::InitializePlayerEquippedItems;
	packetCallbacks[EPacketType::PROJECTILE]			= &AGameModeMainMap::ReceiveReplicatedProjectile;
	packetCallbacks[EPacketType::USINGITEM]				= &AGameModeMainMap::PlayerUseItem;
	packetCallbacks[EPacketType::PLAYERSTATUS]			= &AGameModeMainMap::UpdatePlayerStatus;

	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();

	myNumber = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetPlayerNumber();

	GetWorldTimerManager().SetTimer(playerSpawnDelayTimer, this, &AGameModeMainMap::PlayerSpawnAfterDelay, 0.5f);

	// 좀비 캐릭터 스폰 및 풀링

	zombiePooler->SpawnPoolableActor(2);

	projectilePooler->SpawnPoolableActor(10);
}

void AGameModeMainMap::ProcessPacket()
{
	check(clientSocket);
	if (clientSocket->messageQ.IsEmpty())
		return;

	std::stringstream recvStream;
	clientSocket->messageQ.Dequeue(recvStream);

	int packetType = -1;
	recvStream >> packetType;
	if (packetCallbacks.find(static_cast<EPacketType>(packetType)) == packetCallbacks.end())
	{
		PLOG(TEXT("Invalid packet number! : type number %d"), packetType);
	}
	else
	{
		(this->*packetCallbacks[static_cast<EPacketType>(packetType)])(recvStream);
	}
}

void AGameModeMainMap::SpawnNewPlayerCharacter(std::stringstream& recvStream)
{
	PlayerInitialInfoSet playerInitialInfoSet;
	playerInitialInfoSet.Deserialize(recvStream);
	
	for (auto& initialInfo : playerInitialInfoSet.playerInitialInfoMap)
	{
		int number = initialInfo.first;
		if (number == myNumber) continue;
		PlayerInitialInfo& info = initialInfo.second;
		APlayerCharacter* newPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>
			(
				APlayerCharacter::StaticClass(),
				FVector(info.characterInfo.vectorX, info.characterInfo.vectorY, info.characterInfo.vectorZ),
				FRotator(info.characterInfo.pitch, info.characterInfo.yaw, info.characterInfo.roll)
			);
		newPlayerCharacter->SetPlayerNumber(number);
		newPlayerCharacter->SetPlayerID(FString(UTF8_TO_TCHAR(info.playerID.c_str())));

		for (auto& equipped : info.equippedItems)
		{
			auto itemActor = itemManager->CreatePlayersEquippedItem(equipped);
			itemManager->ItemEquipped(FString(UTF8_TO_TCHAR(equipped.itemID.c_str())), itemActor);
			newPlayerCharacter->AttachItemActor(itemActor);
			itemActor->ActivateEquipMode();
		}

		playerCharacterMap.Add(number, newPlayerCharacter);
	}
}

void AGameModeMainMap::SynchronizePlayers(std::stringstream& recvStream)
{
	PlayerInfoSet playerInfoSet;
	recvStream >> playerInfoSet;
	
	double end = GetWorld()->GetTimeSeconds();

	for (auto& playerInfo : playerInfoSet.characterInfoMap)
	{
		if (playerInfo.first != myNumber && playerCharacterMap.Find(playerInfo.first))
		{
			APlayerCharacter* character = playerCharacterMap[playerInfo.first];
			CharacterInfo& info = playerInfo.second;
			if (IsValid(character))
			{
				character->SetActorLocation(FVector(info.vectorX, info.vectorY, info.vectorZ));
				const FVector lastLocation = FVector(info.vectorX, info.vectorY, info.vectorZ);
				const FVector lastVelocity = FVector(info.velocityX, info.velocityY, info.velocityZ);
				character->DeadReckoningMovement(lastLocation, lastVelocity, (end - info.ratencyStart) / 2.f);
			}
		}
	}
}

void AGameModeMainMap::SynchronizeZombies(std::stringstream& recvStream)
{
	ZombieInfoSet synchZombieInfoSet;
	recvStream >> synchZombieInfoSet;

	for (auto& info : synchZombieInfoSet.zombieInfoMap)
	{
		const ZombieInfo& zombieInfo = info.second;
		TWeakObjectPtr<AZombieCharacter> zombie = nullptr;

		if (zombieCharacterMap.Find(info.first) == nullptr)
		{
			auto actor = zombiePooler->GetPooledActor();
			check(actor.IsValid());
			zombie = Cast<AZombieCharacter>(actor);
			zombie->SetNumber(info.first);
			zombie->ActivateActor();
			zombieCharacterMap.Add(info.first, zombie);
		}
		else
		{
			zombie = MakeWeakObjectPtr<AZombieCharacter>(zombieCharacterMap[info.first].Get());
		}

		// 콜백 함수로 
		if (info.second.recvInfoBitMask & (1 << static_cast<int>(ZIBT::TargetNumber)))
		{
			if (info.second.targetNumber >= 0 && playerCharacterMap.Find(info.second.targetNumber))
			{
				zombie->SetTarget(playerCharacterMap[info.second.targetNumber]);
			}
		}
		zombie->SetZombieInfo(info.second);
	}
}

void AGameModeMainMap::PlayerItemPickUp(std::stringstream& recvStream)
{
	int playerID = -1;
	string itemID;
	recvStream >> playerID >> itemID;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));
	
	if (playerID == myNumber)
	{
		bool rotated = false;
		FTile addedPoint;

		recvStream >> rotated;
		recvStream >> addedPoint.X >> addedPoint.Y;

		TWeakObjectPtr<UItemObject> itemObj = itemManager->GetItemObject(fItemID);
		if (itemObj.IsValid())
		{
			if (rotated) itemObj->Rotate();
			itemManager->ItemPickedUp(fItemID);
			myController->AddItemToInventory(itemObj, addedPoint);
		}
	}
	else
	{
		itemManager->ItemPickedUpOtherPlayer(playerCharacterMap[playerID], fItemID);
	}
}

void AGameModeMainMap::PlayerItemGridPointUpdate(std::stringstream& recvStream)
{
	string itemID;
	int xPoint, yPoint;
	bool isRotated;
	recvStream >> itemID >> xPoint >> yPoint >> isRotated;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

	TWeakObjectPtr<UItemObject> itemObj = itemManager->GetItemObject(fItemID);
	if (itemObj->IsRotated() != isRotated) itemObj->Rotate();
	if (itemObj.IsValid())
	{
		myController->UpdateItemInventoryGrid(itemObj, xPoint, yPoint);
	}
}

void AGameModeMainMap::PlayerItemEquip(std::stringstream& recvStream)
{
	string itemID;
	int playerID = -1, boxNumber;
	bool result = false;
	recvStream >> playerID >> itemID >> boxNumber >> result;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

	TWeakObjectPtr<AItemBase> itemActor = itemManager->GetItemActor(fItemID);
	check(itemActor.IsValid());

	if (playerID == myNumber)
	{
		APlayerControllerMainMap* myPlayerController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (result)
		{
			myPlayerController->EquipItem(boxNumber, itemActor);
		}
		else
		{
			myPlayerController->RestoreInventoryUI(itemActor->GetItemObject());
		}
	}
	else
	{
		playerCharacterMap[playerID]->AttachItemActor(itemActor);
	}
	if (result)
	{
		itemManager->ItemEquipped(fItemID, itemActor);
	}
}

void AGameModeMainMap::PlayerUnequipItem(std::stringstream& recvStream)
{
	string itemID;
	int playerNumber;
	bool result;
	recvStream >> playerNumber >> itemID >> result;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

	if (result)
	{
		bool isRotated;
		int topLeftX, topLeftY;
		recvStream >> isRotated >> topLeftX >> topLeftY;

		auto itemActor = itemManager->GetItemActorInField(fItemID);
		itemManager->ItemPickedUp(fItemID);

		if (playerNumber == myNumber)
		{
			myController->UnequipItemAndAddToInventory(itemActor, { topLeftX,topLeftY });
		}
		else
		{
			playerCharacterMap[playerNumber]->DettachItemActor(itemActor);
		}
	}
	else
	{
		auto itemObj = itemManager->GetItemObject(fItemID);
		myController->RestoreEquipmentUI(itemObj);
	}
}

void AGameModeMainMap::PlayerItemDrop(std::stringstream& recvStream)
{
	string itemID;
	int playerNumber = -1;
	recvStream >> playerNumber >> itemID;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

	TWeakObjectPtr<AItemBase> droppedItem = itemManager->GetItemActor(fItemID);
	if (droppedItem.IsValid() && playerCharacterMap.Find(playerNumber))
	{
		droppedItem->GetItemObject()->ResetOwner();
		DropItem(playerCharacterMap[playerNumber], droppedItem);
	}
}

void AGameModeMainMap::PlayerDropEquippedItem(std::stringstream& recvStream)
{
	string itemID;
	int playerNumber = -1;
	recvStream >> playerNumber >> itemID;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

	TWeakObjectPtr<AItemBase> droppedItem = itemManager->GetItemActorInField(fItemID);
	check(droppedItem.IsValid());
	if (playerNumber == myNumber)
	{
		myController->UnequipItem(droppedItem);
	}
	else
	{
		playerCharacterMap[playerNumber]->DettachItemActor(droppedItem);
	}
	droppedItem->GetItemObject()->ResetOwner();
	DropItem(playerCharacterMap[playerNumber], droppedItem);
}

void AGameModeMainMap::InitializeWorld(std::stringstream& recvStream)
{
	while (1)
	{
		int packetType = -1;
		recvStream >> packetType;
		if (packetCallbacks.find(static_cast<EPacketType>(packetType)) == packetCallbacks.end())
		{
			PLOG(TEXT("Invalid packet number! : type number %d"), packetType);
			break;
		}
		else
		{
			(this->*packetCallbacks[static_cast<EPacketType>(packetType)])(recvStream);
		}
	}
}

void AGameModeMainMap::SynchronizeOtherPlayerInputAction(std::stringstream& recvStream)
{
	int playerNumber = -1, inputType = -1, weaponType = -1;
	recvStream >> playerNumber >> inputType;
	if (playerCharacterMap.Find(playerNumber))
	{
		playerCharacterMap[playerNumber]->DoPlayerInputAction(inputType, weaponType);
	}
}

void AGameModeMainMap::PlayWrestlingResultAction(std::stringstream& recvStream)
{
	bool wrestlingResult = false;
	int playerNumber = -1;
	recvStream >> playerNumber >> wrestlingResult;

	if (playerCharacterMap.Find(playerNumber))
	{
		if (playerNumber == myNumber)
		{
			APlayerControllerMainMap* myPlayerController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			myPlayerController->WrestlingEnd(wrestlingResult);
		}
		else
		{
			if (wrestlingResult)
			{
				playerCharacterMap[playerNumber]->SuccessToBlocking();
			}
			else
			{
				playerCharacterMap[playerNumber]->FailedToResist();
			}
		}
	}
}

void AGameModeMainMap::StartPlayerWrestling(std::stringstream& recvStream)
{
	int playerNumber = -1;
	recvStream >> playerNumber;

	if (playerCharacterMap.Find(playerNumber))
	{
		if (playerNumber == myNumber)
		{
			APlayerControllerMainMap* myPlayerController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			myPlayerController->WrestlingStart();
		}
		else
		{
			playerCharacterMap[playerNumber]->SetWrestlingOn();
		}
	}
}

void AGameModeMainMap::ItemPickUpOtherPlayer(std::stringstream& recvStream)
{
	string itemID;
	int xIndex, yIndex;
	recvStream >> itemID >> xIndex >> yIndex;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

	TWeakObjectPtr<UItemObject> itemObj = itemManager->GetItemObject(fItemID);
	myController->UpdateItemInventoryGrid(itemObj, xIndex, yIndex);
}

void AGameModeMainMap::ProcessDisconnectedPlayer(std::stringstream& recvStream)
{
	int playerNumber = -1;
	recvStream >> playerNumber;
	if (playerCharacterMap.Find(playerNumber))
	{
		APlayerCharacter* player = playerCharacterMap[playerNumber];
		playerCharacterMap.Remove(playerNumber);
		player->Destroy();
	}
}

void AGameModeMainMap::ProcessPlayerDead(std::stringstream& recvStream)
{
	int playerNumber = -1;
	recvStream >> playerNumber;

	if (playerNumber == myNumber)
	{
		APlayerControllerMainMap* myPlayerController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		myPlayerController->PlayerDead();
	}
	else
	{
		if (playerCharacterMap.Find(playerNumber))
		{
			playerCharacterMap[playerNumber]->PlayerDead();
		}
	}
}

void AGameModeMainMap::RespawnPlayer(std::stringstream& recvStream)
{
	int playerNumber = -1;
	CharacterInfo characterInfo;
	recvStream >> playerNumber >> characterInfo;
	if (playerCharacterMap.Find(playerNumber))
	{
		playerCharacterMap[playerNumber]->SetActorLocation(FVector(characterInfo.vectorX, characterInfo.vectorY, characterInfo.vectorZ));
		playerCharacterMap[playerNumber]->SetActorRotation(FRotator(characterInfo.pitch, characterInfo.yaw, characterInfo.roll));
		playerCharacterMap[playerNumber]->PlayerRespawn(playerNumber == myNumber);
	}
}

void AGameModeMainMap::ProcessZombieDead(std::stringstream& recvStream)
{
	int zombieNumber = -1;
	recvStream >> zombieNumber;

	if(zombieCharacterMap.Find(zombieNumber))
	{
		zombieCharacterMap[zombieNumber]->ZombieDead();
		zombieCharacterMap.Remove(zombieNumber);
	}
}

void AGameModeMainMap::SpawnItems(std::stringstream& recvStream)
{
	// 패킷은 아이템의 id, key, 위치 배열
	int size = 0;
	recvStream >> size;
	for (int i = 0; i < size; i++)
	{
		string itemID;
		int itemKey = -1;
		recvStream >> itemID >> itemKey;
		FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

		FVector location;
		recvStream >> location.X >> location.Y >> location.Z;

		itemManager->SpawnItem(fItemID, itemKey, location);
	}
}

void AGameModeMainMap::InitializePlayerPossessedItems(std::stringstream& recvStream)
{
	int size;
	recvStream >> size;
	PossessedItem possessed;
	for (int i = 0; i < size; i++)
	{
		recvStream >> possessed;
		auto itemObj = itemManager->CreatePlayersPossessedItem(possessed);
		myController->AddItemToInventory(itemObj, { possessed.topLeftX, possessed.topLeftY });
	}
}

void AGameModeMainMap::InitializePlayerEquippedItems(std::stringstream& recvStream)
{
	int size;
	recvStream >> size;
	EquippedItem equipped;
	for (int i = 0; i < size; i++)
	{
		recvStream >> equipped;
		auto itemActor = itemManager->CreatePlayersEquippedItem(equipped);
		itemManager->ItemEquipped(FString(UTF8_TO_TCHAR(equipped.itemID.c_str())), itemActor);
		APlayerControllerMainMap* myPlayerController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		myPlayerController->EquipItem(equipped.slotNumber, itemActor);
		itemActor->ActivateEquipMode();
	}
}

void AGameModeMainMap::PlayerSpawnAfterDelay()
{
	// 내 클라이언트 캐릭터 스폰 및 컨트롤러 할당
	APlayerCharacter* myPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator);
	myController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(myController.Get());
	myController->Possess(myPlayerCharacter);
	playerCharacterMap.Add(myNumber, myPlayerCharacter);
}

void AGameModeMainMap::ReceiveReplicatedProjectile(std::stringstream& recvStream)
{
	FVector location;
	FRotator rotation;
	recvStream >> location.X >> location.Y >> location.Z;
	recvStream >> rotation.Pitch >> rotation.Yaw >> rotation.Roll;

	auto projectile = GetProjectile();
	check(projectile.IsValid());

	projectile->SetActorLocation(location);
	projectile->SetActorRotation(rotation);
	projectile->ActivateActor();
}

void AGameModeMainMap::PlayerUseItem(std::stringstream& recvStream)
{
	int playerNumber = -1;
	string itemID;
	recvStream >> playerNumber >> itemID;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));


}

void AGameModeMainMap::UpdatePlayerStatus(std::stringstream& recvStream)
{
	int playerNumber = -1;
	PlayerStatus status;
	recvStream >> playerNumber >> status;

	if (playerNumber == myNumber)
	{
		myController->UpdateHealth(status.health);
	}
	else
	{

	}
}

void AGameModeMainMap::DropItem(TWeakObjectPtr<APlayerCharacter> dropper, TWeakObjectPtr<AItemBase> droppedItem)
{
	FVector spawnLocation = dropper->GetActorLocation();
	FVector EndLocation = spawnLocation;
	EndLocation.Z = -1000.f;
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel
	(
		hit,
		spawnLocation,
		EndLocation,
		ECC_Visibility
	);
	droppedItem->SetActorLocation(hit.ImpactPoint);
	droppedItem->ActivateActor();
}

TWeakObjectPtr<AProjectileBase> AGameModeMainMap::GetProjectile() const
{
	return Cast<AProjectileBase>(projectilePooler->GetPooledActor());
}

void AGameModeMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	ProcessPacket();
}

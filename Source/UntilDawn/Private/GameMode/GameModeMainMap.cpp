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
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/ZombieState.h"
#include "Enums/PoolableActorType.h"
#include "Interface/PoolableActor.h"

AGameModeMainMap::AGameModeMainMap()
{
	PrimaryActorTick.bCanEverTick = true;

	itemManager = CreateDefaultSubobject<UItemManager>(TEXT("Item Manager"));

	zombiePooler = CreateDefaultSubobject<UActorPooler>(TEXT("Zombie Pooler"));

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
	packetCallbacks[EPacketType::ITEMTOPICKUP]			= &AGameModeMainMap::PlayerItemPickUp;
	packetCallbacks[EPacketType::ITEMGRIDPOINTUPDATE]	= &AGameModeMainMap::PlayerItemGridPointUpdate;
	packetCallbacks[EPacketType::ITEMTOEQUIP]			= &AGameModeMainMap::PlayerItemEquip;
	packetCallbacks[EPacketType::ITEMTODROP]			= &AGameModeMainMap::PlayerItemDrop;
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

	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();

	myNumber = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetPlayerNumber();

	GetWorldTimerManager().SetTimer(playerSpawnDelayTimer, this, &AGameModeMainMap::PlayerSpawnAfterDelay, 0.5f);

	// 좀비 캐릭터 스폰 및 풀링

	zombiePooler->SpawnPoolableActor(0, AZombieCharacter::StaticClass(), 2);
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
	PlayerInfoSetEx playerInfoSetEx;
	playerInfoSetEx.InputStreamWithID(recvStream);
	
	for (auto& kv : playerInfoSetEx.characterInfoMap)
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
		newPlayerCharacter->SetPlayerNumber(number);
		newPlayerCharacter->SetPlayerID(FString(UTF8_TO_TCHAR(playerInfoSetEx.playerIDMap[number].c_str())));
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
			CharacterInfo& info = playerInfo.second.characterInfo;
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
			zombie = Cast<AZombieCharacter>(zombiePooler->GetPooledActor(0));
			if (zombie == nullptr)
			{
				zombiePooler->SpawnPoolableActor(0, AZombieCharacter::StaticClass(), 1);
				zombie = Cast<AZombieCharacter>(zombiePooler->GetPooledActor(0));
			}
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
	int playerID = -1, itemID = -1;
	recvStream >> playerID >> itemID;
	
	if (playerID == myNumber)
	{
		bool rotated = false;
		FTile addedPoint;

		recvStream >> rotated;
		recvStream >> addedPoint.X >> addedPoint.Y;

		TWeakObjectPtr<UItemObject> itemObj = itemManager->GetItemObject(itemID);
		if (itemObj.IsValid())
		{
			if (rotated) itemObj->Rotate();
			itemManager->ItemPickedUp(itemID);
			playerCharacterMap[myNumber]->AddItemToInventory(itemObj, addedPoint);
		}
	}
	else
	{
		// 어느 플레이어가 획득했는지도 추가할지
		itemManager->ItemPickedUpOtherPlayer(itemID);
	}
}

void AGameModeMainMap::PlayerItemGridPointUpdate(std::stringstream& recvStream)
{
	int itemID, xPoint, yPoint;
	bool isRotated;
	recvStream >> itemID >> xPoint >> yPoint >> isRotated;

	TWeakObjectPtr<UItemObject> itemObj = itemManager->GetItemObject(itemID);
	if (itemObj->IsRotated() != isRotated) itemObj->Rotate();
	if (itemObj.IsValid())
	{
		playerCharacterMap[myNumber]->UpdateItemInventoryGrid(itemObj, xPoint, yPoint);
	}
}

void AGameModeMainMap::PlayerItemEquip(std::stringstream& recvStream)
{
	int playerID = -1, itemID = -1, boxNumber;
	bool result = false;
	recvStream >> playerID >> itemID >> boxNumber >> result;

	TWeakObjectPtr<AItemBase> item = itemManager->GetItemActor(itemID);
	check(item.IsValid());

	if (playerID == myNumber)
	{
		APlayerControllerMainMap* myPlayerController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (result)
		{
			myPlayerController->ProcessItemEquipInUI(boxNumber, item);
		}
		else
		{
			myPlayerController->RestoreInventoryUI(item->GetItemObject());
		}
	}
	if (result)
	{
		// ui창에 장착하는 것이므로 액터를 장비 장착용으로 활성화(콜리전X)
		playerCharacterMap[playerID]->AttachItemActor(item);
		item->ActivateEquipMode();
	}
}

void AGameModeMainMap::PlayerItemDrop(std::stringstream& recvStream)
{
	int playerID = -1, itemID = -1;
	recvStream >> playerID >> itemID;

	TWeakObjectPtr<AItemBase> droppedItem = itemManager->GetItemActor(itemID);
	if (droppedItem.IsValid() && playerCharacterMap.Find(playerID))
	{
		DropItem(playerCharacterMap[playerID], droppedItem);
	}
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
	int playerNumber = -1, inputType = -1;
	recvStream >> playerNumber >> inputType;
	if (playerCharacterMap.Find(playerNumber))
	{
		playerCharacterMap[playerNumber]->DoPlayerInputAction(inputType);
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
	int itemID, xIndex, yIndex;
	recvStream >> itemID >> xIndex >> yIndex;
	TWeakObjectPtr<UItemObject> itemObj = itemManager->GetItemObject(itemID);
	playerCharacterMap[myNumber]->UpdateItemInventoryGrid(itemObj, xIndex, yIndex);
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
		int id = -1, key = -1;
		recvStream >> id >> key;
		FVector location;
		recvStream >> location.X >> location.Y >> location.Z;

		itemManager->SpawnItem(id, key, location);
	}
}

void AGameModeMainMap::PlayerSpawnAfterDelay()
{
	// 내 클라이언트 캐릭터 스폰 및 컨트롤러 할당
	APlayerCharacter* myPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator);
	APlayerController* myPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	myPlayerController->Possess(myPlayerCharacter);
	playerCharacterMap.Add(myNumber, myPlayerCharacter);
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

TWeakObjectPtr<AItemBase> AGameModeMainMap::GetItemActor(const int itemID)
{
	return itemManager->GetItemActor(itemID);
}

void AGameModeMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	ProcessPacket();
}

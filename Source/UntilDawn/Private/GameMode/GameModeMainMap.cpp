// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeMainMap.h"
#include "GameSystem/ActorSpawner.h"
#include "GameSystem/ActorPooler.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"
#include "UI/Main/HUDMainMap.h"
#include "Zombie/ZombieCharacter.h"
#include "Item/ItemBase.h"
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/ZombieState.h"
#include "Enums/PoolableActorType.h"
#include "Interface/PoolableActor.h"

AGameModeMainMap::AGameModeMainMap()
{
	PrimaryActorTick.bCanEverTick = true;

	actorSpawner = CreateDefaultSubobject<UActorSpawner>(TEXT("Actor Spawner"));

	zombiePooler = CreateDefaultSubobject<UActorPooler>(TEXT("Zombie Pooler"));

	itemPooler = CreateDefaultSubobject<UActorPooler>(TEXT("Item Pooler"));

	PlayerControllerClass = APlayerControllerMainMap::StaticClass();
	DefaultPawnClass = nullptr;
	HUDClass = AHUDMainMap::StaticClass();
}

void AGameModeMainMap::BeginPlay()
{
	Super::BeginPlay();

	//packetCallbacks = std::vector<void (AGameModeMainMap::*)(std::stringstream&)>(PACKETTYPE_MAX);
	//packetCallbacks[static_cast<int>(EPacketType::SPAWNPLAYER)] = &AGameModeMainMap::SpawnNewPlayerCharacter;
	//packetCallbacks[static_cast<int>(EPacketType::SYNCHPLAYER)] = &AGameModeMainMap::SynchronizePlayers;
	//packetCallbacks[static_cast<int>(EPacketType::SYNCHITEM)]	= &AGameModeMainMap::SynchronizeItems;
	//packetCallbacks[static_cast<int>(EPacketType::INITIALINFO)]	= &AGameModeMainMap::InitializeWorld;

	packetCallbacks = std::vector<void (AGameModeMainMap::*)(char*)>(PACKETTYPE_MAX);
	packetCallbacks[static_cast<int>(EPacketType::SPAWNPLAYER)] = &AGameModeMainMap::SpawnNewPlayerCharacter;
	packetCallbacks[static_cast<int>(EPacketType::SYNCHPLAYER)] = &AGameModeMainMap::SynchronizePlayers;

	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	clientSocket->SetGameMode(this);

	myNumber = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetPlayerNumber();

	GetWorldTimerManager().SetTimer(playerSpawnDelayTimer, this, &AGameModeMainMap::PlayerSpawnAfterDelay, 0.5f);

	// 좀비 캐릭터 스폰 및 풀링
	zombiePooler->SetPoolSize(2);
	actorSpawner->SpawnActor(zombiePooler->GetPoolSize(), EPoolableActorType::Zombie, zombiePooler->GetActorPool());

	// 아이템 스폰 및 풀링
	itemPooler->SetPoolSize(5);
	actorSpawner->SpawnActor(itemPooler->GetPoolSize(), EPoolableActorType::MeleeWeapon, itemPooler->GetActorPool());
}

void AGameModeMainMap::ReceivePacket(std::stringstream& recvStream)
{
	//FScopeLock Lock(&criticalSection);
	messageQ.push(std::move(recvStream));
}

void AGameModeMainMap::ProcessPacket()
{
	/*if (clientSocket->IsMessageQueueEmpty())
		return;
	//FScopeLock Lock(&criticalSection);
	std::stringstream recvStream;
	clientSocket->GetDataInMessageQueue(recvStream);
	//WLOG(TEXT("en"));
	//PLOG(TEXT("%s"), UTF8_TO_TCHAR(recvStream.str().c_str()));
	//Lock.Unlock();

	int packetType = -1;
	recvStream >> packetType;
	if (packetType >= 0 && packetType < PACKETTYPE_MAX)
	{
		if (packetCallbacks[packetType] == nullptr)
		{
			ELOG(TEXT("Callback is nullptr!"));
			PLOG(TEXT("cb %d"), packetType);
			PLOG(TEXT("%s"), UTF8_TO_TCHAR(recvStream.str().c_str()));
		}
		else
		{
			(this->*packetCallbacks[packetType])(recvStream);
		}
	}
	else
	{
		ELOG(TEXT("Invalid packet number!"));
	}*/

	/*int bufferSize = clientSocket->messageQ.unsafe_size();

	while (remainData < bufferSize)
	{
		while (remainData < PACKET_SIZE && remainData < bufferSize)
		{
			clientSocket->messageQ.try_pop(data[remainData++]);
		}

		while (remainData > 0 && data[0] <= remainData)
		{
			if (ProcessPacket2(data) == false)
				return;
			remainData -= data[0];
			bufferSize -= data[0];
			memcpy(data, data + data[0], PACKET_SIZE - data[0]);
		}
	}*/

	int bufferSize = clientSocket->messageQ.unsafe_size();
	if (bufferSize)
	{
		mVec.clear();
		clientSocket->messageQ.try_pop(mVec);

		int head = 0;
		size = 0;
		memcpy((void*)&size, &mVec[0], sizeof(size));
		//PLOG(TEXT("size %d"), size);
		head += sizeof(size);

		ProcessPacket2(mVec.data()+ head);

		//int type = 0;
		//memcpy((void*)&type, &mVec[0]+head, sizeof(type));
		//head += sizeof(type);
		//PLOG(TEXT("type is %d"), type);

		//if (type == 2)
		//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, FString::Printf(TEXT("%d"), type));

		//while (remainData < PACKET_SIZE && remainData < bufferSize)
		//{
		//	clientSocket->messageQ.try_pop(data[remainData++]);
		//}
		//
		//if (remainData >= INT_SIZE)
		//{
		//	size = 0;
		//	memcpy((void*)&size, data, sizeof(size));
		//	dataHead += INT_SIZE;
		//	remainData -= size;
		//}
		//
		//if (remainData >= size)
		//{
		//	PLOG(TEXT("size %d"), size);
		//	//ProcessPacket2(data + dataHead);
		//	ZeroMemory(&data, remainData + size);
		//	remainData = 0;
		//}
	}

	//if (bufferSize)
	//{
	//	int size = 0, head = 0, type = 0, idx = 0;
	//	while (idx < sizeof(size))
	//	{
	//		clientSocket->messageQ.try_pop(data[idx++]);
	//	}
	//	memcpy((void*)&size, data, sizeof(size));
	//	head += sizeof(size);
	//
	//	idx = 0;
	//	while (idx < sizeof(size))
	//	{
	//		clientSocket->messageQ.try_pop(data[head + idx++]);
	//	}
	//	
	//	if (ProcessPacket2(data + head))
	//	{
	//		ZeroMemory(&data, PACKET_SIZE);
	//	}
	//}
}

bool AGameModeMainMap::ProcessPacket2(char* p)
{
	int type = 0, head = 0;
	memcpy((void*)&type, p, sizeof(type));
	head += sizeof(type);
	PLOG(TEXT("type is %d"), type);
	if (type >= 0 && type < PACKETTYPE_MAX)
	{
		if (packetCallbacks[type] == nullptr)
		{
			ELOG(TEXT("Callback is nullptr!"));
			PLOG(TEXT("cb %d"), type);
			//PLOG(TEXT("%s"), UTF8_TO_TCHAR(recvStream.str().c_str()));
		}
		else
		{
			(this->*packetCallbacks[type])(p + head);
			PLOG(TEXT("true type %d"), type);
			return true;
		}
	}

	return false;
}

void AGameModeMainMap::SpawnNewPlayerCharacter(char * p)
{
	//PlayerInfoSetEx playerInfoSetEx2;
	//playerInfoSetEx2.InputStreamWithID(recvStream);
	//
	//for (auto& kv : playerInfoSetEx2.characterInfoMap)
	//{
	//	int number = kv.first;
	//	if (number == myNumber) continue;
	//	CharacterInfo& info = kv.second.characterInfo;
	//	APlayerCharacter* newPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>
	//		(
	//			APlayerCharacter::StaticClass(),
	//			FVector(info.vectorX, info.vectorY, info.vectorZ),
	//			FRotator(info.pitch, info.yaw, info.roll)
	//		);
	//	newPlayerCharacter->SetPlayerNumber(number);
	//	newPlayerCharacter->SetPlayerID(FString(UTF8_TO_TCHAR(playerInfoSetEx2.playerIDMap[number].c_str())));
	//	playerCharacterMap.Add(number, newPlayerCharacter);
	//}

	/*
	Infos 사이즈 타입 카운트
	for 카운트
		CInfo
	*/
	//std::memcpy( outData, mBuffer + mHead, inByteCount );

	int head = 0;
	Infos infos;
	std::memcpy((void*)&infos.count, p + head, sizeof(infos.count));
	head += sizeof(infos.count);
	infos.vec = std::vector<CInfo>(infos.count);
	for (int i = 0; i < infos.count; i++)
	{
		std::memcpy((void*)&infos.vec[i], p + head, sizeof(infos.vec[i]));
		head += sizeof(infos.vec[i]);
	}
	
	for (auto& info : infos.vec)
	{
		int number = info.number;
		if (number == myNumber) continue;
		APlayerCharacter* newPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>
			(
				APlayerCharacter::StaticClass(),
				FVector(info.loc.X, info.loc.Y, info.loc.Z),
				FRotator(info.rot.p, info.rot.y, info.rot.r)
			);
		newPlayerCharacter->SetPlayerNumber(number);
		playerCharacterMap.Add(number, newPlayerCharacter);
	}
}

void AGameModeMainMap::SynchronizePlayers(char* p)
{
	//PlayerInfoSet playerInfoSet2;
	//recvStream >> playerInfoSet2;
	//
	//for (auto& playerInfo : playerInfoSet2.characterInfoMap)
	//{
	//	if (playerInfo.first != myNumber && playerCharacterMap.Find(playerInfo.first))
	//	{
	//		APlayerCharacter* character = playerCharacterMap[playerInfo.first];
	//		CharacterInfo& info = playerInfo.second.characterInfo;
	//		if (IsValid(character))
	//		{
	//			character->AddMovementInput(FVector(info.velocityX, info.velocityY, info.velocityZ));
	//			character->SetActorRotation(FRotator(info.pitch, info.yaw, info.roll));
	//			character->SetActorLocation(FVector(info.vectorX, info.vectorY, info.vectorZ));
	//		}
	//	}
	//	if (playerInfo.second.flag)
	//	{
	//		const int bitMax = static_cast<int>(PIBTS::MAX);
	//		for (int bit = 0; bit < bitMax; bit++)
	//		{
	//			if (playerInfo.second.recvInfoBitMask & (1 << bit))
	//			{
	//				ProcessPlayerInfo(playerInfo.first, playerInfo.second, bit);
	//			}
	//		}
	//	}
	//}

	int head = 0;
	Infos infos;
	std::memcpy((void*)&infos.count, p + head, sizeof(infos.count));
	head += sizeof(infos.count);
	infos.vec = std::vector<CInfo>(infos.count);
	for (int i = 0; i < infos.count; i++)
	{
		std::memcpy((void*)&infos.vec[i], p + head, sizeof(infos.vec[i]));
		head += sizeof(infos.vec[i]);
	}
	
	for (auto& info : infos.vec)
	{
		if (info.number != myNumber && playerCharacterMap.Find(info.number))
		{
			APlayerCharacter* character = playerCharacterMap[info.number];
			if (IsValid(character))
			{
				character->AddMovementInput(FVector(info.vel.X, info.vel.Y, info.vel.Z));
				character->SetActorRotation(FRotator(info.rot.p, info.rot.y, info.rot.r));
				character->SetActorLocation(FVector(info.loc.X, info.loc.Y, info.loc.Z));
			}
		}
	}
}

void AGameModeMainMap::SynchronizeItems(std::stringstream& recvStream)
{
	ItemInfoSet itemInfoSet2;
	recvStream >> itemInfoSet2;

	for (auto& info : itemInfoSet2.itemInfoMap)
	{
		const FItemInfo& itemInfo = info.second;
		AItemBase* item = nullptr;
		if (itemMap.Find(info.first) == nullptr)
		{
			item = Cast<AItemBase>(itemPooler->GetPooledActor());
			if (item == nullptr)
			{
				actorSpawner->SpawnActor(1, EPoolableActorType::MeleeWeapon, itemPooler->GetActorPool());
				item = Cast<AItemBase>(itemPooler->GetPooledActor());
			}
			item->SetNumber(info.first);
			itemMap.Add(info.first, item);
		}
		else
		{
			item = itemMap[info.first];
		}

		item->SetItemInfo(itemInfo);
		if (itemInfo.state != EItemState::Activated)
		{
			itemMap.Remove(item->GetNumber());
		}
	}
}

void AGameModeMainMap::InitializeWorld(std::stringstream& recvStream)
{
	//while (1)
	//{
	//	int packetType = -1;
	//	recvStream >> packetType;
	//	if (packetType >= 0 && packetType < PACKETTYPE_MAX)
	//	{
	//		if (packetCallbacks[packetType] == nullptr)
	//		{
	//			ELOG(TEXT("Callback is nullptr!"));
	//			PLOG(TEXT("cb %d"), packetType);
	//		}
	//		else
	//		{
	//			(this->*packetCallbacks[packetType])(recvStream);
	//		}
	//	}
	//	else break;
	//}
}

void AGameModeMainMap::PlayerSpawnAfterDelay()
{
	// 내 클라이언트 캐릭터 스폰 및 컨트롤러 할당
	APlayerCharacter* myPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator);
	APlayerController* myPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	myPlayerController->Possess(myPlayerCharacter);
	playerCharacterMap.Add(myNumber, myPlayerCharacter);
}

void AGameModeMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	//if (!messageQ.empty())
	//{
	//	ProcessPacket();
	//}

	if (clientSocket)
	{
		ProcessPacket();
	}

	//if (!playerToDelete.IsEmpty())
	//{
	//	DestroyPlayer();
	//}
	//if (playerInfoSetEx)
	//{
	//	SpawnNewPlayerCharacter();
	//}
	//if (playerInfoSet)
	//{
	//	SynchronizePlayersInfo();
	//}
	//if (zombieInfoSet)
	//{
	//	SynchronizeZombieInfo();
	//}
	//if (itemInfoSet)
	//{
	//	SynchronizeItemInfo();
	//}
	//if (!wrestlingResultQ.IsEmpty())
	//{
	//	ProcessWrestlingResult();
	//}
	//if (!wrestlingStartQ.IsEmpty())
	//{
	//	StartPlayerWrestlingAction();
	//}
	//if (!destroyItemQ.IsEmpty())
	//{
	//	DestroyItem();
	//}
	//if (!pickUpItemQ.IsEmpty())
	//{
	//	PickUpItem();
	//}
	//if (!deadZombieQ.IsEmpty())
	//{
	//	ProcessZombieDead();
	//}
	//if (!deadPlayerQ.IsEmpty())
	//{
	//	ProcessPlayerDead();
	//}
	//if (respawnNumber != -1)
	//{
	//	RespawnPlayer();
	//}
}

void AGameModeMainMap::DestroyPlayer()
{
	int number;
	while (!playerToDelete.IsEmpty())
	{
		playerToDelete.Dequeue(number);
		APlayerCharacter* tempCharacter = playerCharacterMap[number];
		tempCharacter->Destroy();
		playerCharacterMap.Remove(number);
	}
}

void AGameModeMainMap::ReceiveNewPlayerInfo(PlayerInfoSetEx* newPlayerInfoSet)
{
	playerInfoSetEx = newPlayerInfoSet;
}

void AGameModeMainMap::ReceiveOtherPlayersInfo(PlayerInfoSet* synchPlayerInfoSet)
{
	playerInfoSet = synchPlayerInfoSet;
}

void AGameModeMainMap::ReceiveZombieInfo(ZombieInfoSet* synchZombieInfoSet)
{
	zombieInfoSet = synchZombieInfoSet;
}

void AGameModeMainMap::ReceiveItemInfo(ItemInfoSet* synchItemInfoSet)
{
	itemInfoSet = synchItemInfoSet;
}

void AGameModeMainMap::DestroyItem(const int itemNumber)
{
	if (itemMap.Find(itemNumber))
	{
		destroyItemQ.Enqueue(itemNumber);
	}
}

void AGameModeMainMap::PickUpItem(const int itemNumber)
{
	if (itemMap.Find(itemNumber))
	{
		pickUpItemQ.Enqueue(itemNumber);
	}
}

void AGameModeMainMap::ReceiveDeadZombieNumber(const int zombieNumber)
{
	if (zombieCharacterMap.Find(zombieNumber))
	{
		deadZombieQ.Enqueue(zombieNumber);
	}
}

void AGameModeMainMap::ReceiveDeadPlayerNumber(const int playerNumber)
{
	if (playerCharacterMap.Find(playerNumber))
	{
		deadPlayerQ.Enqueue(playerNumber);
	}
}

void AGameModeMainMap::ReceiveRespawnPlayerNumber(const int playerNumber, CharacterInfo info)
{
	respawnNumber = playerNumber;
	respawnInfo = info;
}

void AGameModeMainMap::ReceiveDisconnectedPlayerInfo(const int playerNumber)
{
	if (playerCharacterMap.Find(playerNumber))
	{
		APlayerCharacter* tempCharacter = playerCharacterMap[playerNumber];
		if (IsValid(tempCharacter))
		{
			playerToDelete.Enqueue(playerNumber);
		}
	}
}

void AGameModeMainMap::SynchronizeOtherPlayerInputAction(const int playerNumber, const int inputType)
{
	if (playerCharacterMap.Find(playerNumber))
	{
		APlayerCharacter* tempCharacter = playerCharacterMap[playerNumber];
		if (IsValid(tempCharacter))
		{
			tempCharacter->DoPlayerInputAction(inputType);
		}
	}
}

void AGameModeMainMap::PlayWrestlingResultAction(const int playerNumber, const bool wrestlingResult)
{
	wrestlingResultQ.Enqueue({ playerNumber, wrestlingResult });
}

void AGameModeMainMap::ReceiveWrestlingPlayer(const int playerNumber)
{
	wrestlingStartQ.Enqueue(playerNumber);
}

void AGameModeMainMap::SpawnNewPlayerCharacter()
{
	for (auto& kv : playerInfoSetEx->characterInfoMap)
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
		newPlayerCharacter->SetPlayerID(FString(UTF8_TO_TCHAR(playerInfoSetEx->playerIDMap[number].c_str())));
		playerCharacterMap.Add(number, newPlayerCharacter);
	}
	playerInfoSetEx = nullptr;
}

void AGameModeMainMap::SynchronizePlayersInfo()
{
	for (auto& playerInfo : playerInfoSet->characterInfoMap)
	{
		const int bitMax = static_cast<int>(PIBTS::MAX);
		for (int bit = 0; bit < bitMax; bit++)
		{
			if (playerInfo.second.recvInfoBitMask & (1 << bit))
			{
				ProcessPlayerInfo(playerInfo.first, playerInfo.second, bit);
			}
		}
		if (playerInfo.first != myNumber && playerCharacterMap.Find(playerInfo.first))
		{
			APlayerCharacter* character = playerCharacterMap[playerInfo.first];
			CharacterInfo& info = playerInfo.second.characterInfo;
			if (IsValid(character))
			{
				character->AddMovementInput(FVector(info.velocityX, info.velocityY, info.velocityZ));
				character->SetActorRotation(FRotator(info.pitch, info.yaw, info.roll));
				character->SetActorLocation(FVector(info.vectorX, info.vectorY, info.vectorZ));
			}
		}
	}
	playerInfoSet = nullptr;
}

void AGameModeMainMap::ProcessPlayerInfo(const int playerNumber, const PlayerInfo& info, const int bitType)
{
	PIBTS type = static_cast<PIBTS>(bitType);
	switch (type)
	{
		case PIBTS::WrestlingState:
		{
			break;
		}
	}
}

void AGameModeMainMap::SynchronizeZombieInfo()
{
	for (auto& info : zombieInfoSet->zombieInfoMap)
	{
		const ZombieInfo& zombieInfo = info.second;
		AZombieCharacter* zombie = nullptr;

		if (zombieCharacterMap.Find(info.first) == nullptr)
		{
			zombie = Cast<AZombieCharacter>(zombiePooler->GetPooledActor());
			if (zombie == nullptr)
			{
				actorSpawner->SpawnActor(1, EPoolableActorType::Zombie, zombiePooler->GetActorPool());
				zombie = Cast<AZombieCharacter>(zombiePooler->GetPooledActor());
			}
			zombie->SetNumber(info.first);
			zombie->ActivateActor();
			zombieCharacterMap.Add(info.first, zombie);
		}
		else
		{
			zombie = zombieCharacterMap[info.first];
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
	zombieInfoSet = nullptr;
}

void AGameModeMainMap::SynchronizeItemInfo()
{
	for (auto& info : itemInfoSet->itemInfoMap)
	{
		const FItemInfo& itemInfo = info.second;
		AItemBase* item = nullptr;
		if (itemMap.Find(info.first) == nullptr)
		{
			item = Cast<AItemBase>(itemPooler->GetPooledActor());
			if (item == nullptr)
			{
				actorSpawner->SpawnActor(1, EPoolableActorType::MeleeWeapon, itemPooler->GetActorPool());
				item = Cast<AItemBase>(itemPooler->GetPooledActor());
			}
			item->SetNumber(info.first);
			itemMap.Add(info.first, item);
		}
		else
		{
			item = itemMap[info.first];
		}

		item->SetItemInfo(itemInfo);
		if (itemInfo.state != EItemState::Activated)
		{
			itemMap.Remove(item->GetNumber());
		}
	}
	itemInfoSet = nullptr;
}

void AGameModeMainMap::ProcessWrestlingResult()
{
	std::pair<int, bool> wrestlingResult;
	wrestlingResultQ.Dequeue(wrestlingResult);

	if (playerCharacterMap.Find(wrestlingResult.first))
	{
		APlayerCharacter* character = playerCharacterMap[wrestlingResult.first];
		character->PlayPushingZombieMontage(wrestlingResult.second);
		character->SetWrestlingOff();
	}
}

void AGameModeMainMap::StartPlayerWrestlingAction()
{
	int wrestlingPlayer;
	wrestlingStartQ.Dequeue(wrestlingPlayer);

	if (playerCharacterMap.Find(wrestlingPlayer))
	{
		playerCharacterMap[wrestlingPlayer]->SetWrestlingOn();
	}
}

void AGameModeMainMap::DestroyItem()
{
	int number = 0;
	while (!destroyItemQ.IsEmpty())
	{
		destroyItemQ.Dequeue(number);
		itemMap[number]->DeactivateActor();
	}
}

void AGameModeMainMap::PickUpItem()
{
	int number = 0;
	while (!pickUpItemQ.IsEmpty())
	{
		pickUpItemQ.Dequeue(number);
		playerCharacterMap[myNumber]->AddItemToInv(itemMap[number]);
		//itemMap[number]->DeactivateActor();
	}
}

void AGameModeMainMap::ProcessZombieDead()
{
	int number = 0;
	while (!deadZombieQ.IsEmpty())
	{
		deadZombieQ.Dequeue(number);
		zombieCharacterMap[number]->ZombieDead();
		zombieCharacterMap.Remove(number);
	}
}

void AGameModeMainMap::ProcessPlayerDead()
{
	int number = 0;
	while (!deadPlayerQ.IsEmpty())
	{
		deadPlayerQ.Dequeue(number);
		if (number == myNumber)
		{
			APlayerControllerMainMap* myPlayerController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			myPlayerController->PlayerDead();
		}
		else
		{
			if (playerCharacterMap.Find(number))
			{
				playerCharacterMap[number]->PlayerDead();
			}
		}
	}
}

void AGameModeMainMap::RespawnPlayer()
{
	if (playerCharacterMap.Find(respawnNumber))
	{
		WLOG(TEXT("dead"));
		playerCharacterMap[respawnNumber]->SetActorLocation(FVector(respawnInfo.vectorX, respawnInfo.vectorY, respawnInfo.vectorZ));
		playerCharacterMap[respawnNumber]->SetActorRotation(FRotator(respawnInfo.pitch, respawnInfo.yaw, respawnInfo.roll));
		playerCharacterMap[respawnNumber]->PlayerRespawn(respawnNumber == myNumber);
	}
	respawnNumber = -1;
}

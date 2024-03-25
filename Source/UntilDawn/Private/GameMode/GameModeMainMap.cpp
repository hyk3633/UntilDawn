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

	if (!playerToDelete.IsEmpty())
	{
		DestroyPlayer();
	}
	if (playerInfoSetEx)
	{
		SpawnNewPlayerCharacter();
	}
	if (playerInfoSet)
	{
		SynchronizePlayersInfo();
	}
	if (zombieInfoSet)
	{
		SynchronizeZombieInfo();
	}
	if (itemInfoSet)
	{
		SynchronizeItemInfo();
	}
	if (!wrestlingResultQ.IsEmpty())
	{
		ProcessWrestlingResult();
	}
	if (!wrestlingStartQ.IsEmpty())
	{
		StartPlayerWrestlingAction();
	}
	if (!destroyItemQ.IsEmpty())
	{
		DestroyItem();
	}
	if (!pickUpItemQ.IsEmpty())
	{
		PickUpItem();
	}
	if (!deadZombieQ.IsEmpty())
	{
		ProcessZombieDead();
	}
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
		newPlayerCharacter->SpawnDefaultController();
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

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
#include "Item/ItemObject/ItemPermanent.h"
#include "Item/Projectile/ProjectileBase.h"
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/ZombieState.h"
#include "Enums/PoolableActorType.h"
#include "Engine/DataTable.h"
#include "Components/CapsuleComponent.h"
#include "Interface/PoolableActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTargetActor.h"

using std::string;

AGameModeMainMap::AGameModeMainMap()
{
	PrimaryActorTick.bCanEverTick = true;

	itemManager = CreateDefaultSubobject<UItemManager>(TEXT("Item Manager"));

	zombiePooler = CreateDefaultSubobject<UActorPooler>(TEXT("Zombie Pooler"));
	zombiePooler->SetActorClass(AZombieCharacter::StaticClass());

	static ConstructorHelpers::FObjectFinder<UDataTable> zombieAssetDataTableRef(TEXT("DataTable'/Game/_Assets/DataTable/DT_ZombieAsset.DT_ZombieAsset'"));
	if (zombieAssetDataTableRef.Succeeded()) zombieAssetDataTable = zombieAssetDataTableRef.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> itemFieldRotationDataTableRef(TEXT("DataTable'/Game/_Assets/DataTable/DT_ItemFieldRotation.DT_ItemFieldRotation'"));
	if (itemFieldRotationDataTableRef.Succeeded()) itemFieldRotationDataTable = itemFieldRotationDataTableRef.Object;

	projectilePooler = CreateDefaultSubobject<UActorPooler>(TEXT("Projectile Pooler"));
	projectilePooler->SetActorClass(AProjectileBase::StaticClass());

	PlayerControllerClass = APlayerControllerMainMap::StaticClass();
	DefaultPawnClass = nullptr;
	HUDClass = AHUDMainMap::StaticClass();

	static ConstructorHelpers::FClassFinder<APlayerCharacter> playerCharacterRef(TEXT("/Script/Engine.Blueprint'/Game/_Assets/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (playerCharacterRef.Class)
	{
		playerCharacterClass = playerCharacterRef.Class;
	}
}

void AGameModeMainMap::BeginPlay()
{
	Super::BeginPlay();

	check(playerCharacterClass);
	
	// 월드 정보 초기화
	packetCallbacks[EPacketType::WORLD_INITIAL_INFO]		= &AGameModeMainMap::InitializeWorld;
	
	// 플레이어 정보 및 상태 동기화
	packetCallbacks[EPacketType::SPAWN_PLAYER]				= &AGameModeMainMap::SpawnNewPlayerCharacter;
	packetCallbacks[EPacketType::PLAYER_INITIAL_INFO]		= &AGameModeMainMap::InitializePlayerInitialInfo;
	packetCallbacks[EPacketType::PLAYER_INVENTORY]			= &AGameModeMainMap::InitializePlayerPossessedItems;
	packetCallbacks[EPacketType::PLAYER_EQUIPMENT]			= &AGameModeMainMap::InitializePlayerEquippedItems;
	packetCallbacks[EPacketType::SYNCH_PLAYER]				= &AGameModeMainMap::SynchronizePlayers;
	packetCallbacks[EPacketType::HEALTH_CHANGED]			= &AGameModeMainMap::UpdateCharacterHealth;
	packetCallbacks[EPacketType::PLAYER_DEAD]				= &AGameModeMainMap::ProcessPlayerDead;
	packetCallbacks[EPacketType::PLAYER_RESPAWN]			= &AGameModeMainMap::RespawnPlayer;
	packetCallbacks[EPacketType::PLAYER_DISCONNECTED]		= &AGameModeMainMap::ProcessDisconnectedPlayer;
	
	// 플레이어 좀비 레슬링
	packetCallbacks[EPacketType::WRESTLING_START]			= &AGameModeMainMap::StartWrestling;
	packetCallbacks[EPacketType::WRESTLING_RESULT]			= &AGameModeMainMap::PlayWrestlingResultAction;
	packetCallbacks[EPacketType::WRESTLING_CANCELED]		= &AGameModeMainMap::CancelWrestling;
	
	// 좀비의 플레이어 공격 
	packetCallbacks[EPacketType::ZOMBIE_HITS_ME]			= &AGameModeMainMap::ProcessZombieHit;
	
	// 플레이어의 인벤토리의 아이템 그리드 위치 변경
	packetCallbacks[EPacketType::ITEM_GRID_POINT_UPDATE]	= &AGameModeMainMap::UpdateInventoryItemGridPoint;
	
	// 플레이어의 공격 동기화
	packetCallbacks[EPacketType::ACTIVATE_WEAPON_ABILITY]	= &AGameModeMainMap::ActivateWeaponAbility;
	packetCallbacks[EPacketType::ATTACK_RESULT]				= &AGameModeMainMap::ProcessAttackResult;
	packetCallbacks[EPacketType::KICKED_CHARACTERS]			= &AGameModeMainMap::ProcessKickedCharacters;
	packetCallbacks[EPacketType::PROJECTILE]				= &AGameModeMainMap::ReplicateProjectile;
	
	// 아이템 동기화
	packetCallbacks[EPacketType::SPAWN_ITEM]				= &AGameModeMainMap::SpawnItems;
	packetCallbacks[EPacketType::PICKUP_ITEM]				= &AGameModeMainMap::PickUpItem;
	packetCallbacks[EPacketType::DROP_ITEM]					= &AGameModeMainMap::DropItem;
	packetCallbacks[EPacketType::EQUIP_ITEM]				= &AGameModeMainMap::EquipItem;
	packetCallbacks[EPacketType::UNEQUIP_ITEM]				= &AGameModeMainMap::UnequipItem;
	packetCallbacks[EPacketType::DROP_EQUIPPED_ITEM]		= &AGameModeMainMap::DropEquippedItem;
	packetCallbacks[EPacketType::ARM_WEAPON]				= &AGameModeMainMap::ArmWeapon;
	packetCallbacks[EPacketType::DISARM_WEAPON]				= &AGameModeMainMap::DisarmWeapon;
	packetCallbacks[EPacketType::CHANGE_WEAPON]				= &AGameModeMainMap::ChangeWeapon;
	packetCallbacks[EPacketType::USING_ITEM]				= &AGameModeMainMap::UseItem;
	
	// 좀비 동기화
	packetCallbacks[EPacketType::SYNCH_ZOMBIE]				= &AGameModeMainMap::SynchronizeZombies;
	packetCallbacks[EPacketType::ZOMBIE_DEAD]				= &AGameModeMainMap::ProcessZombieDead;

	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();

	myID = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetPlayerID();
	myNumber = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetPlayerNumber();

	GetWorldTimerManager().SetTimer(playerSpawnDelayTimer, this, &AGameModeMainMap::SpawnPlayerAfterDelay, 0.5f);

	// 좀비 캐릭터 스폰 및 풀링

	zombiePooler->SpawnPoolableActor(10);

	projectilePooler->SpawnPoolableActor(30);
}

void AGameModeMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (bProcessPacket)
	{
		ProcessPacket();
	}
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

void AGameModeMainMap::SpawnNewPlayerCharacter(std::stringstream& recvStream)
{
	PlayerInitialInfoSet playerInitialInfoSet;
	playerInitialInfoSet.Deserialize(recvStream);

	for (auto& initialInfo : playerInitialInfoSet.playerInitialInfoMap)
	{
		const int playerNumber = initialInfo.first;
		if (playerNumber == myNumber)
			continue;

		PlayerInitialInfo& info = initialInfo.second;
		APlayerCharacter* newPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>
			(
				playerCharacterClass,
				GetPlayerStartTransform()
			);

		newPlayerCharacter->SpawnDefaultController();
		newPlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(FName("RemotePlayer"));
		newPlayerCharacter->SetPlayerIDAndNumber(FString(UTF8_TO_TCHAR(info.playerID.c_str())), playerNumber);
		newPlayerCharacter->InitializeHealthWidget();
		newPlayerCharacter->SetMaxHealth(info.playerStatus.health);
		newPlayerCharacter->SetHealth(info.playerStatus.health);

		for (auto& equipped : info.equippedItems)
		{
			auto itemActor = itemManager->GetPlayersEquippedItem(equipped);
			itemManager->ItemEquipped(playerNumber, FString(UTF8_TO_TCHAR(equipped.itemID.c_str())), itemActor);
			newPlayerCharacter->EquipItem(itemActor, equipped.slotNumber);
			if (equipped.isArmed)
			{
				newPlayerCharacter->ArmWeapon(itemActor);
			}
		}
		playerCharacterMap.Add(playerNumber, newPlayerCharacter);
	}
}

void AGameModeMainMap::InitializePlayerInitialInfo(std::stringstream& recvStream)
{
	int health = 0, stamina = 0, rows = 0, columns = 0;
	recvStream >> health >> stamina >> rows >> columns;
	playerCharacterMap[myNumber]->SetMaxHealth(health);
	myController->SetStamina(stamina);
	myController->SetRowColumn(rows, columns);
}

void AGameModeMainMap::InitializePlayerPossessedItems(std::stringstream& recvStream)
{
	int size;
	recvStream >> size;
	PossessedItem possessed;
	for (int i = 0; i < size; i++)
	{
		recvStream >> possessed;
		auto itemObj = itemManager->GetPlayersPossessedItem(possessed);
		myController->AddItemToInventory(itemObj, { possessed.topLeftX, possessed.topLeftY });
	}
}

void AGameModeMainMap::InitializePlayerEquippedItems(std::stringstream& recvStream)
{
	int size;
	recvStream >> size;

	for (int i = 0; i < size; i++)
	{
		EquippedItem equipped;
		recvStream >> equipped;
		auto itemActor = itemManager->GetPlayersEquippedItem(equipped);
		itemManager->ItemEquipped(myNumber, FString(UTF8_TO_TCHAR(equipped.itemID.c_str())), itemActor);
		APlayerControllerMainMap* myPlayerController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		myPlayerController->EquipItem(equipped.slotNumber, itemActor);
	}
}

void AGameModeMainMap::SynchronizePlayers(std::stringstream& recvStream)
{
	PlayerInfoSet playerInfoSet;
	float pitch = 0.f;
	recvStream >> playerInfoSet >> pitch;

	for (auto& playerInfo : playerInfoSet.characterInfoMap)
	{
		
		if (playerInfo.first != myNumber && playerCharacterMap.Find(playerInfo.first))
		{
			APlayerCharacter* character = playerCharacterMap[playerInfo.first];
			CharacterInfo& info = playerInfo.second;
	
			if (IsValid(character))
			{
				const FVector velocity = FVector(info.velocityX, info.velocityY, info.velocityZ);
				character->AddMovementInput(velocity);
				character->SetActorRotation(FRotator(info.pitch, info.yaw, info.roll));
				character->SetActorLocation(FVector(info.vectorX, info.vectorY, info.vectorZ));
				character->SetPitch(pitch);
				character->SetTargetSpeed(velocity.Length());
			}
		}
	}
}

void AGameModeMainMap::UpdateCharacterHealth(std::stringstream& recvStream)
{
	int characterNumber = -1;
	bool isPlayer = false;
	float health = 0;

	recvStream >> characterNumber >> health >> isPlayer;
	if (isPlayer)
	{
		if (characterNumber == myNumber)
		{
			myController->UpdateHealth(health);
		}
		else
		{
			playerCharacterMap[characterNumber]->SetHealth(health);
		}
	}
	else
	{
		zombieCharacterMap[characterNumber]->UpdateHealth(health);
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
		playerCharacterMap[playerNumber]->PlayerRespawn();
	}
}

void AGameModeMainMap::ProcessDisconnectedPlayer(std::stringstream& recvStream)
{
	int playerNumber = -1;
	recvStream >> playerNumber;
	if (playerCharacterMap.Find(playerNumber))
	{
		auto player = playerCharacterMap[playerNumber];
		playerCharacterMap.Remove(playerNumber);
		itemManager->RemovePlayersItems(playerNumber);
		player->Destroy();
	}
}

void AGameModeMainMap::StartWrestling(std::stringstream& recvStream)
{
	int playerNumber = -1;
	recvStream >> playerNumber;

	if (playerCharacterMap.Find(playerNumber))
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(playerCharacterMap[playerNumber], UD_EVENT_WRESTLING_START, FGameplayEventData());
	}
}

void AGameModeMainMap::PlayWrestlingResultAction(std::stringstream& recvStream)
{
	bool wrestlingResult = false;
	int playerNumber = -1;
	recvStream >> playerNumber >> wrestlingResult;

	if (playerCharacterMap.Find(playerNumber))
	{
		if (wrestlingResult)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(playerCharacterMap[playerNumber], UD_EVENT_WRESTLING_BLOCKED, FGameplayEventData());
		}
		else
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(playerCharacterMap[playerNumber], UD_EVENT_WRESTLING_BITED, FGameplayEventData());
		}
	}
}

void AGameModeMainMap::CancelWrestling(std::stringstream& recvStream)
{
	int playerNumber = -1;
	recvStream >> playerNumber;
	if (playerNumber == myNumber)
	{
		myController->CancelWrestling();
	}
	if (playerCharacterMap.Find(playerNumber))
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(playerCharacterMap[playerNumber], UD_EVENT_CHARACTER_HITREACTION, FGameplayEventData());
	}
}

void AGameModeMainMap::UpdateInventoryItemGridPoint(std::stringstream& recvStream)
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

void AGameModeMainMap::ActivateWeaponAbility(std::stringstream& recvStream)
{
	int playerNumber = 0, inputType = 0;
	recvStream >> playerNumber >> inputType;
	if (playerCharacterMap.Find(playerNumber))
	{
		playerCharacterMap[playerNumber]->TryActivateWeaponAbility(StaticCast<EInputType>(inputType));
	}
}

void AGameModeMainMap::ProcessAttackResult(std::stringstream& recvStream)
{
	FHitInfo hitInfo;
	string itemID;
	recvStream >> itemID >> hitInfo;

	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));
	auto itemObj = itemManager->GetItemObject(fItemID);

	FHitResult hitResult;
	hitResult.ImpactPoint = hitInfo.hitLocation;
	hitResult.ImpactNormal = hitInfo.hitRotation.Vector();

	AActor* targetActor = nullptr;
	if (hitInfo.isPlayer)
	{
		if (playerCharacterMap.Find(hitInfo.characterNumber))
		{
			targetActor = playerCharacterMap[hitInfo.characterNumber];
		}
	}
	else
	{
		if (zombieCharacterMap.Find(hitInfo.characterNumber))
		{
			targetActor = zombieCharacterMap[hitInfo.characterNumber].Get();
		}
	}

	if (targetActor == nullptr)
		return;

	FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(hitResult);
	FGameplayEventData payloadData;
	payloadData.TargetData.Add(targetData);
	if (itemObj.IsValid())
	{
		TWeakObjectPtr<UItemPermanent> weaponObj = Cast<UItemPermanent>(itemObj);
		payloadData.EventMagnitude = weaponObj->GetItemSubType();
	}
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(targetActor, UD_EVENT_CHARACTER_HITREACTION, payloadData);

}

void AGameModeMainMap::ProcessKickedCharacters(std::stringstream& recvStream)
{
	int kickerNumber = 0, kickedNumber = 0, opponentNumber = 0;
	bool isPlayer = false, bOpponent = false;
	recvStream >> kickerNumber >> kickedNumber >> isPlayer >> bOpponent;
	if (bOpponent)
	{
		recvStream >> opponentNumber;
	}

	FGameplayEventData payloadData;
	if (playerCharacterMap.Find(kickerNumber))
	{
		FHitResult kickResult;
		kickResult.ImpactPoint = playerCharacterMap[kickerNumber]->GetActorLocation();
		FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(kickResult);
		payloadData.TargetData.Add(targetData);
	}

	ACharacter* kickedCharacter = nullptr;
	ACharacter* opponentCharacter = nullptr;
	if (isPlayer && playerCharacterMap.Find(kickedNumber))
	{
		kickedCharacter = playerCharacterMap[kickedNumber];

		if (kickedNumber == myNumber && playerCharacterMap[kickedNumber]->IsWrestling())
		{
			myController->CancelWrestling();
		}
		if (bOpponent && zombieCharacterMap.Find(opponentNumber))
		{
			opponentCharacter = zombieCharacterMap[opponentNumber].Get();
		}
	}
	else if (zombieCharacterMap.Find(kickedNumber))
	{
		kickedCharacter = zombieCharacterMap[kickedNumber].Get();

		if (bOpponent && playerCharacterMap.Find(opponentNumber))
		{
			opponentCharacter = playerCharacterMap[opponentNumber];
			if (opponentNumber == myNumber && playerCharacterMap[opponentNumber]->IsWrestling())
			{
				myController->CancelWrestling();
			}
		}
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(kickedCharacter, UD_EVENT_CHARACTER_KICKREACTION, payloadData);
	if (opponentCharacter)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(opponentCharacter, UD_EVENT_CHARACTER_KICKREACTION, payloadData);
	}
}

void AGameModeMainMap::ReplicateProjectile(std::stringstream& recvStream)
{
	int number = 0;
	FVector location;
	FRotator rotation;
	recvStream >> number;
	recvStream >> location.X >> location.Y >> location.Z;
	recvStream >> rotation.Pitch >> rotation.Yaw >> rotation.Roll;

	auto projectile = GetProjectile();
	check(projectile.IsValid());

	projectile->SetOwner(playerCharacterMap[number]);
	projectile->SetActorLocation(location);
	projectile->SetActorRotation(rotation);
	projectile->ActivateProjectile();
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

		auto itemActor = itemManager->SpawnItem(fItemID, itemKey);
		SetFieldItemTransform(itemActor, location);
	}
}

void AGameModeMainMap::PickUpItem(std::stringstream& recvStream)
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

void AGameModeMainMap::DropItem(std::stringstream& recvStream)
{
	string itemID;
	int playerNumber = -1;
	bool result = false;
	recvStream >> playerNumber >> itemID >> result;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

	if (result)
	{
		int itemKey = 0, itemQuantity = 0;
		recvStream >> itemKey >> itemQuantity;
		TWeakObjectPtr<AItemBase> droppedItem = itemManager->GetItemActor(fItemID, itemKey, itemQuantity);
		if (droppedItem.IsValid() && playerCharacterMap.Find(playerNumber))
		{
			droppedItem->GetItemObject()->ResetOwner();
			RespawnDroppedItem(playerCharacterMap[playerNumber], droppedItem);
		}
	}
}

void AGameModeMainMap::EquipItem(std::stringstream& recvStream)
{
	string itemID;
	int playerNumber = -1, boxNumber;
	bool result = false;
	recvStream >> playerNumber >> itemID >> boxNumber >> result;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

	TWeakObjectPtr<AItemBase> itemActor = itemManager->GetItemActor(fItemID);
	check(itemActor.IsValid());

	if (playerNumber == myNumber)
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
		playerCharacterMap[playerNumber]->EquipItem(itemActor, boxNumber);
	}
	if (result)
	{
		itemManager->ItemEquipped(playerNumber, fItemID, itemActor);
	}
}

void AGameModeMainMap::UnequipItem(std::stringstream& recvStream)
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
			playerCharacterMap[playerNumber]->UnEquipItem(itemActor);
		}
	}
	else
	{
		auto itemObj = itemManager->GetItemObject(fItemID);
		myController->RestoreEquipmentUI(itemObj);
	}
}

void AGameModeMainMap::DropEquippedItem(std::stringstream& recvStream)
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
		playerCharacterMap[playerNumber]->UnEquipItem(droppedItem);
	}
	RespawnDroppedItem(playerCharacterMap[playerNumber], droppedItem);
}

void AGameModeMainMap::ArmWeapon(std::stringstream& recvStream)
{
	int playerNumber = -1;
	string armedWeaponID;

	recvStream >> playerNumber >> armedWeaponID;
	FString fArmedWeaponID = FString(UTF8_TO_TCHAR(armedWeaponID.c_str()));

	TWeakObjectPtr<AItemBase> armedWeaponActor = itemManager->GetItemActorInField(fArmedWeaponID);
	check(armedWeaponActor.IsValid());

	playerCharacterMap[playerNumber]->ArmWeapon(armedWeaponActor);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(playerCharacterMap[playerNumber], UD_EVENT_CHARACTER_ARMWEAPON, FGameplayEventData());
}

void AGameModeMainMap::DisarmWeapon(std::stringstream& recvStream)
{
	int playerNumber = -1;
	recvStream >> playerNumber;

	playerCharacterMap[playerNumber]->DisarmWeapon();
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(playerCharacterMap[playerNumber], UD_EVENT_CHARACTER_DISARMWEAPON, FGameplayEventData());
}

void AGameModeMainMap::ChangeWeapon(std::stringstream& recvStream)
{
	int playerNumber = -1;
	string changedWeaponID;

	recvStream >> playerNumber >> changedWeaponID;
	FString fChangedWeaponID = FString(UTF8_TO_TCHAR(changedWeaponID.c_str()));

	TWeakObjectPtr<AItemBase> changedWeaponActor = itemManager->GetItemActorInField(fChangedWeaponID);
	check(changedWeaponActor.IsValid());

	playerCharacterMap[playerNumber]->ChangeWeapon(changedWeaponActor);
	FGameplayEventData payloadData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(playerCharacterMap[playerNumber], UD_EVENT_CHARACTER_CHANGEWEAPON, payloadData);
}

void AGameModeMainMap::UseItem(std::stringstream& recvStream)
{
	int playerNumber = -1, consumedAmount = 0;
	string itemID;
	recvStream >> playerNumber >> itemID >> consumedAmount;
	FString fItemID = FString(UTF8_TO_TCHAR(itemID.c_str()));

	itemManager->OtherPlayerUseItem(playerCharacterMap[playerNumber], fItemID, consumedAmount);
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
			zombie->SetSkeletalMesh(GetZombieMesh(info.first));
			zombie->SetActorLocation(info.second.location);
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

void AGameModeMainMap::ProcessZombieHit(std::stringstream& recvStream)
{
	FHitInfo hitInfo;
	recvStream >> hitInfo;

	FHitResult hitResult;
	hitResult.ImpactPoint = hitInfo.hitLocation;
	hitResult.ImpactNormal = hitInfo.hitRotation.Vector();

	AActor* targetActor = nullptr;
	if (playerCharacterMap.Find(hitInfo.characterNumber))
	{
		targetActor = playerCharacterMap[hitInfo.characterNumber];
	}

	if (targetActor == nullptr)
		return;

	FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(hitResult);
	FGameplayEventData payloadData;
	payloadData.TargetData.Add(targetData);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(targetActor, UD_EVENT_CHARACTER_HITBYZOMBIE, payloadData);

}

void AGameModeMainMap::ProcessZombieDead(std::stringstream& recvStream)
{
	int zombieNumber = -1;
	recvStream >> zombieNumber;

	if (zombieCharacterMap.Find(zombieNumber))
	{
		zombieCharacterMap[zombieNumber]->ZombieDead();
		zombieCharacterMap.Remove(zombieNumber);
	}
}

USkeletalMesh* AGameModeMainMap::GetZombieMesh(const int32 zombieNumber)
{
	FZombieAsset* zombieAsset = zombieAssetDataTable->FindRow<FZombieAsset>(*FString::FromInt(zombieNumber % 4), TEXT(""));
	return zombieAsset->skeletalMesh;
}

void AGameModeMainMap::SpawnPlayerAfterDelay()
{
	APlayerCharacter* myPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(playerCharacterClass, GetPlayerStartTransform());
	myPlayerCharacter->SetPlayerIDAndNumber(myID, myNumber);
	myPlayerCharacter->InitializeHealthWidget();
	myController = Cast<APlayerControllerMainMap>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(myController.Get());
	myController->Possess(myPlayerCharacter);
	playerCharacterMap.Add(myNumber, myPlayerCharacter);
	bProcessPacket = true;
}

void AGameModeMainMap::RespawnDroppedItem(TWeakObjectPtr<APlayerCharacter> dropper, TWeakObjectPtr<AItemBase> droppedItem)
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

	SetFieldItemTransform(droppedItem, hit.ImpactPoint);
	droppedItem->GetItemObject()->ResetOwner();
	droppedItem->ActivateFieldMode();
}

TWeakObjectPtr<AProjectileBase> AGameModeMainMap::GetProjectile() const
{
	return Cast<AProjectileBase>(projectilePooler->GetPooledActor());
}

void AGameModeMainMap::SetFieldItemTransform(TWeakObjectPtr<AItemBase> droppedItem, FVector spawnLocation)
{
	FItemFieldRotation* itemFieldInfo = itemFieldRotationDataTable->FindRow<FItemFieldRotation>(*FString::FromInt(droppedItem->GetItemObject()->GetItemInfo().itemKey), TEXT(""));
	spawnLocation.X += itemFieldInfo->location.X;
	spawnLocation.Y += itemFieldInfo->location.Y;
	spawnLocation.Z += itemFieldInfo->location.Z;
	droppedItem->SetActorLocation(spawnLocation);
	droppedItem->SetActorRotation(itemFieldInfo->rotation);
}

FTransform AGameModeMainMap::GetPlayerStartTransform()
{
	return FindPlayerStart(nullptr)->GetActorTransform();
}

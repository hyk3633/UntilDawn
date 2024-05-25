// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Main/PlayerControllerMainMap.h"
#include "Network/ClientSocket.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "GameSystem/InventoryComponent.h"
#include "UI/Main/HUDMainMap.h"
#include "Player/PlayerCharacter.h"
#include "Zombie/ZombieCharacter.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject.h"
#include "Item/ItemObject/ItemRecovery.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

APlayerControllerMainMap::APlayerControllerMainMap()
{
	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> obj_DefaultContext(TEXT("/Game/_Assets/Inputs/IMC_DefaultsController.IMC_DefaultsController"));
	if (obj_DefaultContext.Succeeded()) defaultMappingContext = obj_DefaultContext.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_LeftClick(TEXT("/Game/_Assets/Inputs/Actions/IA_LeftClick.IA_LeftClick"));
	if (obj_LeftClick.Succeeded()) leftClickAction = obj_LeftClick.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_LeftClickHold(TEXT("/Game/_Assets/Inputs/Actions/IA_LeftClickHold.IA_LeftClickHold"));
	if (obj_LeftClickHold.Succeeded()) leftClickHoldAction = obj_LeftClickHold.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_RightClick(TEXT("/Game/_Assets/Inputs/Actions/IA_RightClick.IA_RightClick"));
	if (obj_RightClick.Succeeded()) rightClickAction = obj_RightClick.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_RKey(TEXT("/Game/_Assets/Inputs/Actions/IA_RKey.IA_RKey"));
	if (obj_RKey.Succeeded()) rKeyAction = obj_RKey.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_RKeyHold(TEXT("/Game/_Assets/Inputs/Actions/IA_RKeyHold.IA_RKeyHold"));
	if (obj_RKeyHold.Succeeded()) rKeyHoldAction = obj_RKeyHold.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_EKey(TEXT("/Game/_Assets/Inputs/Actions/IA_EKey.IA_EKey"));
	if (obj_EKey.Succeeded()) eKeyAction = obj_EKey.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_IKey(TEXT("/Game/_Assets/Inputs/Actions/IA_IKey.IA_IKey"));
	if (obj_IKey.Succeeded()) iKeyAction = obj_IKey.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_HKey(TEXT("/Game/_Assets/Inputs/Actions/IA_HKey.IA_HKey"));
	if (obj_HKey.Succeeded()) hKeyAction = obj_HKey.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_Wheel(TEXT("/Game/_Assets/Inputs/Actions/IA_Wheel.IA_Wheel"));
	if (obj_Wheel.Succeeded()) wheelAction = obj_Wheel.Object;
}

void APlayerControllerMainMap::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly gameInputMode;
	SetInputMode(gameInputMode);
}

void APlayerControllerMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	Trace();
}

void APlayerControllerMainMap::Trace()
{
	FVector location, direction;
	GetTraceLocationAndDirection(location, direction);
	CharacterTrace(location, direction);
	ItemTrace(location, direction);
}

void APlayerControllerMainMap::GetTraceLocationAndDirection(FVector& location, FVector& direction)
{
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}
	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	UGameplayStatics::DeprojectScreenToWorld(this, CrosshairLocation, location, direction);
}

void APlayerControllerMainMap::CharacterTrace(const FVector& location, const FVector& direction)
{
	GetWorld()->LineTraceSingleByChannel
	(
		characterHit,
		location,
		location + direction * 5000.f,
		ECC_PlayerTrace
	);
	if (characterHit.bBlockingHit)
	{
		TWeakObjectPtr<APlayerCharacter> otherPlayer = Cast<APlayerCharacter>(characterHit.GetActor());
		if (otherPlayer.IsValid())
		{
			otherPlayer->ShowHealthWidget();
		}
	}
}

void APlayerControllerMainMap::ItemTrace(const FVector& location, const FVector& direction)
{
	GetWorld()->LineTraceSingleByChannel
	(
		itemHit,
		location,
		location + direction * 5000.f,
		ECC_ItemTrace
	);
	if (itemHit.bBlockingHit)
	{
		TWeakObjectPtr<AItemBase> item = Cast<AItemBase>(itemHit.GetActor());
		if (item.IsValid() && item != lookingItem)
		{
			if (lookingItem.IsValid())
			{
				lookingItem->RenderCustomDepthOff();
				lookingItem.Reset();
			}
			lookingItem = item;
			lookingItem->RenderCustomDepthOn();
		}
	}
	else
	{
		if (lookingItem.IsValid())
		{
			lookingItem->RenderCustomDepthOff();
			lookingItem.Reset();
		}
	}
}

void APlayerControllerMainMap::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) 
	{
		EnhancedInputComponent->BindAction(leftClickAction,			ETriggerEvent::Started,   this, &APlayerControllerMainMap::LeftClick);
		EnhancedInputComponent->BindAction(leftClickHoldAction,		ETriggerEvent::Triggered, this, &APlayerControllerMainMap::LeftClickHold);
		EnhancedInputComponent->BindAction(leftClickAction,			ETriggerEvent::Completed, this, &APlayerControllerMainMap::LeftClickEnd);
		EnhancedInputComponent->BindAction(rightClickAction,		ETriggerEvent::Triggered, this, &APlayerControllerMainMap::RightClick);
		EnhancedInputComponent->BindAction(rightClickAction,		ETriggerEvent::Completed, this, &APlayerControllerMainMap::RightClickEnd);
		EnhancedInputComponent->BindAction(rKeyAction,				ETriggerEvent::Completed, this, &APlayerControllerMainMap::RKeyPressed);
		EnhancedInputComponent->BindAction(rKeyHoldAction,			ETriggerEvent::Triggered, this, &APlayerControllerMainMap::RKeyHold);
		EnhancedInputComponent->BindAction(eKeyAction,				ETriggerEvent::Completed, this, &APlayerControllerMainMap::EKeyPressed);
		EnhancedInputComponent->BindAction(iKeyAction,				ETriggerEvent::Completed, this, &APlayerControllerMainMap::IKeyPressed);
		EnhancedInputComponent->BindAction(hKeyAction,				ETriggerEvent::Completed, this, &APlayerControllerMainMap::HKeyPressed);
		EnhancedInputComponent->BindAction(wheelAction,				ETriggerEvent::Completed, this, &APlayerControllerMainMap::WheelRolled);
	}
}

void APlayerControllerMainMap::LeftClick()
{
	const EPermanentItemType weaponType = inventoryComponent->GetCurrentWeaponType();
	if (weaponType != EPermanentItemType::NONE && inventoryComponent->IsWeaponUsable())
	{
		if (myCharacter->LeftClick(weaponType))
		{
			SendPlayerInputAction(EPlayerInputs::LeftClick, weaponType);
		}
	}
}

void APlayerControllerMainMap::LeftClickHold()
{
	const EPermanentItemType weaponType = inventoryComponent->GetCurrentWeaponType();
	if (myCharacter->LeftClickHold(weaponType))
	{
		SendPlayerInputAction(EPlayerInputs::LeftClickHold, weaponType);
	}
}

void APlayerControllerMainMap::LeftClickEnd()
{
	const EPermanentItemType weaponType = inventoryComponent->GetCurrentWeaponType();
	if (myCharacter->LeftClickEnd(weaponType))
	{
		SendPlayerInputAction(EPlayerInputs::LeftClickEnd, weaponType);
	}
}

void APlayerControllerMainMap::RightClick()
{
	const EPermanentItemType weaponType = inventoryComponent->GetCurrentWeaponType();
	if (myCharacter->RightClick(weaponType))
	{
		SendPlayerInputAction(EPlayerInputs::RightClick, weaponType);
	}
}

void APlayerControllerMainMap::RightClickEnd()
{
	const EPermanentItemType weaponType = inventoryComponent->GetCurrentWeaponType();
	if (myCharacter->RightClickEnd(weaponType))
	{
		SendPlayerInputAction(EPlayerInputs::RightClickEnd, weaponType);
	}
}

void APlayerControllerMainMap::RKeyPressed()
{
	const EPermanentItemType currentWeaponType = inventoryComponent->GetCurrentWeaponType();
	if (currentWeaponType == EPermanentItemType::NONE)
	{
		auto itemActor = inventoryComponent->ArmRecentWeapon();
		if (myCharacter->ArmWeapon(itemActor))
		{
			DWeaponArmed.ExecuteIfBound(itemActor->GetItemObject()->GetRotatedIcon());
			clientSocket->ArmWeapon(itemActor->GetItemID());
		}
	}
}

void APlayerControllerMainMap::RKeyHold()
{
	const EPermanentItemType weaponType = inventoryComponent->GetCurrentWeaponType();
	if (weaponType != EPermanentItemType::NONE && myCharacter->DisarmWeapon())
	{
		inventoryComponent->DisarmWeapon();
		clientSocket->DisarmWeapon();
		DWeaponArmed.ExecuteIfBound(nullptr);
	}
}

void APlayerControllerMainMap::EKeyPressed()
{
	check(myCharacter);
	if (myCharacter->GetWrestling())
	{
		DEKeyPressed.ExecuteIfBound();
	}
	else if(lookingItem.IsValid())
	{
		SendPickedItemInfo(lookingItem->GetItemID());
		lookingItem.Reset();
	}
}

void APlayerControllerMainMap::IKeyPressed()
{
	DIKeyPressed.ExecuteIfBound();
}

void APlayerControllerMainMap::HKeyPressed()
{
	check(myCharacter);
	if (myCharacter->HKeyPressed())
	{
		inventoryComponent->UsingConsumableItemOfType(EItemMainType::RecoveryItem);
	}
}

void APlayerControllerMainMap::WheelRolled()
{
	if (inventoryComponent->IsAnyWeaponArmed())
	{
		auto changedWeaponActor = inventoryComponent->ChangeWeapon();
		if (changedWeaponActor.IsValid())
		{
			DWeaponArmed.ExecuteIfBound(changedWeaponActor->GetItemObject()->GetRotatedIcon());
			myCharacter->ChangeWeapon(changedWeaponActor);
			clientSocket->ChangeWeapon(changedWeaponActor->GetItemID());
		}
	}
}

void APlayerControllerMainMap::WrestlingStart()
{
	check(myCharacter);
	myCharacter->SetWrestlingOn();
	DWrestlingStart.ExecuteIfBound();
}

void APlayerControllerMainMap::WrestlingEnd(const bool wrestlingResult)
{
	check(myCharacter);
	if (wrestlingResult)
	{
		myCharacter->SuccessToBlocking();
	}
	else
	{
		myCharacter->FailedToResist();
	}
}

void APlayerControllerMainMap::SuccessToBlocking()
{
	SendPlayerBlockingResult(true);
}

void APlayerControllerMainMap::FailedToBlocking()
{
	SendPlayerBlockingResult(false);
}

void APlayerControllerMainMap::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	myCharacter = Cast<APlayerCharacter>(pawn);
	check(myCharacter);
	// 서버에 게임 맵에 접속했음을 알림
	myCharacter->DZombieInRange.BindUFunction(this, FName("SendInRangeZombie"));
	myCharacter->DZombieOutRange.BindUFunction(this, FName("SendOutRangeZombie"));
	myCharacter->DZombieHitsMe.BindUFunction(this, FName("SendZombieHitsMe"));
	myCharacter->UpdatePlayerInfo();

	inventoryComponent->SetCharacter(myCharacter);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(defaultMappingContext, 1);
	}

	DHealthChanged.ExecuteIfBound(myCharacter->GetHealthPercentage());

	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	check(clientSocket);
	clientSocket->NotifyAccessingGame(myCharacter->GetPlayerInfo());
	GetWorldTimerManager().SetTimer(SynchronizeTimer, this, &APlayerControllerMainMap::SynchronizePlayerInfo, 0.2f, true);
}

void APlayerControllerMainMap::SendPlayerInputAction(const EPlayerInputs inputType, const EPermanentItemType weaponType)
{
	clientSocket->SendPlayerInputAction(static_cast<int>(inputType), static_cast<int>(weaponType));
}

void APlayerControllerMainMap::SendInRangeZombie(int zombieNumber)
{
	clientSocket->SendInRangeZombie(zombieNumber);
}

void APlayerControllerMainMap::SendOutRangeZombie(int zombieNumber)
{
	clientSocket->SendOutRangeZombie(zombieNumber);
}

void APlayerControllerMainMap::SendZombieHitsMe(int zombieNumber, bool bResult)
{
	clientSocket->SendZombieHitsMe(zombieNumber, bResult);
}

void APlayerControllerMainMap::SendPlayerBlockingResult(const bool isSuccessToBlocking)
{
	clientSocket->SendPlayerBlockingResult(isSuccessToBlocking);
}

void APlayerControllerMainMap::SendPickedItemInfo(const FString itemID)
{
	clientSocket->SendPickedItemInfo(itemID);
}

void APlayerControllerMainMap::AddItemToInventory(TWeakObjectPtr<UItemObject> itemObj, const FTile& addedPoint)
{
	inventoryComponent->AddItemAt(itemObj, addedPoint);

	itemObj->SetOwnerController(this);
	itemObj->SetOwnerCharacter(myCharacter);
}

void APlayerControllerMainMap::NotifyToServerUpdateItemGridPoint(const FString itemID, const int xPoint, const int yPoint, const bool isRotated)
{
	clientSocket->UpdateItemGridPoint(itemID, xPoint, yPoint, isRotated);
}

void APlayerControllerMainMap::UpdateItemInventoryGrid(TWeakObjectPtr<UItemObject> itemObj, const int xIndex, const int yIndex)
{
	inventoryComponent->AddItemAt(itemObj, { xIndex, yIndex });
}

void APlayerControllerMainMap::SendItemInfoToEquip(const FString itemID, const int boxNumber)
{
	clientSocket->SendItemInfoToEquip(itemID, boxNumber);
}

void APlayerControllerMainMap::EquipItem(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor)
{
	DEquipItem.ExecuteIfBound(itemActor->GetItemObject().Get(), boxNumber);
	inventoryComponent->EquipItem(boxNumber, itemActor);
	myCharacter->EquipItem(itemActor, boxNumber);

	itemActor->GetItemObject()->SetOwnerController(this);
	itemActor->GetItemObject()->SetOwnerCharacter(myCharacter);
}

void APlayerControllerMainMap::NotifyToServerUnequipItem(const FString itemID, const FTile& addedPoint)
{
	clientSocket->UnequipItem(itemID, addedPoint.X, addedPoint.Y);
}

void APlayerControllerMainMap::UnequipItemAndAddToInventory(TWeakObjectPtr<AItemBase> itemActor, const FTile& addedPoint)
{
	if (myCharacter->GetArmedWeapon() == itemActor)
	{
		DWeaponArmed.ExecuteIfBound(nullptr);
	}
	myCharacter->UnEquipItem(itemActor);
	inventoryComponent->UnequipItem(itemActor);
	AddItemToInventory(itemActor->GetItemObject(), addedPoint);
}

void APlayerControllerMainMap::UnequipItem(TWeakObjectPtr<AItemBase> itemActor)
{
	if (myCharacter->GetArmedWeapon() == itemActor)
	{
		DWeaponArmed.ExecuteIfBound(nullptr);
	}
	myCharacter->UnEquipItem(itemActor);
	inventoryComponent->UnequipItem(itemActor);
}

void APlayerControllerMainMap::RestoreInventoryUI(TWeakObjectPtr<UItemObject> itemObj)
{
	inventoryComponent->AddItemAt(itemObj);
}

void APlayerControllerMainMap::RestoreEquipmentUI(TWeakObjectPtr<UItemObject> itemObj)
{
	const int slotNumber = inventoryComponent->GetSlotNumber(itemObj);
	DEquipItem.ExecuteIfBound(itemObj.Get(), slotNumber);
}

void APlayerControllerMainMap::DropEquippedItem(const FString itemID)
{
	check(clientSocket);
	clientSocket->DropEquippedItem(itemID);
}

void APlayerControllerMainMap::DropInventoryItem(const FString itemID)
{
	check(clientSocket);
	clientSocket->DropInventoryItem(itemID);
}

void APlayerControllerMainMap::SendHittedCharacters(TArray<FHitResult>& hits)
{
	check(clientSocket);
	TArray<TPair<int, bool>> hittedCharacters;
	for (auto& hit : hits)
	{
		if (hit.bBlockingHit == false)
			continue;
		TWeakObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(hit.GetActor());
		if (player.IsValid())
		{
			// 플레이어인 경우 구분하는 flag를 true로
			hittedCharacters.Add(TPair<int, bool>(player->GetPlayerNumber(), true));
		}
		else
		{
			TWeakObjectPtr<AZombieCharacter> zombie = Cast<AZombieCharacter>(hit.GetActor());
			if (zombie.IsValid())
			{
				// 좀비인 경우 구분하는 flag를 false로
				hittedCharacters.Add(TPair<int, bool>(zombie->GetNumber(), false));
			}
		}
	}
	if (hittedCharacters.Num())
	{
		clientSocket->SendHittedCharacters(hittedCharacters);
	}
}

void APlayerControllerMainMap::PlayerDead()
{
	check(myCharacter);
	myCharacter->PlayerDead();
	GetWorldTimerManager().SetTimer(respawnRequestTimer, this, &APlayerControllerMainMap::respawnRequestAfterDelay, 3.f);
	DPlayerDead.ExecuteIfBound();
}

void APlayerControllerMainMap::StartAttack()
{
	inventoryComponent->Attack();
}

void APlayerControllerMainMap::ReplicateProjectile(const FVector& location, const FRotator& rotation)
{
	check(clientSocket);
	clientSocket->ReplicateProjectile(location, rotation);
}

TWeakObjectPtr<UItemObject> APlayerControllerMainMap::GetItemObjectOfType(const EItemMainType itemType)
{
	return inventoryComponent->GetItemObjectOfType(itemType);
}

void APlayerControllerMainMap::SendItemUsing(const FString& itemID, const int consumedAmount)
{
	check(clientSocket);
	clientSocket->SendItemUsing(itemID, consumedAmount);
}

void APlayerControllerMainMap::ItemExhausted(TWeakObjectPtr<UItemObject> itemObj)
{
	inventoryComponent->RemoveItem(itemObj);
}

void APlayerControllerMainMap::UpdateHealth(const float health)
{
	check(myCharacter);
	myCharacter->SetHealth(health);
	DHealthChanged.ExecuteIfBound(myCharacter->GetHealthPercentage());
}

void APlayerControllerMainMap::SetRowColumn(const int r, const int c)
{
	inventoryComponent->SetRowColumn(r, c);
	GetHUD<AHUDMainMap>()->StartHUD();
}

void APlayerControllerMainMap::SynchronizePlayerInfo()
{
	check(myCharacter);
	check(clientSocket);
	myCharacter->UpdatePlayerInfo();
	clientSocket->SynchronizeMyCharacterInfo(myCharacter->GetPlayerInfo());
}

void APlayerControllerMainMap::respawnRequestAfterDelay()
{
	check(clientSocket);
	clientSocket->SendRespawnRequest();
}

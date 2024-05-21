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
	inventoryComponent->SetColumns(6);
	inventoryComponent->SetRows(15);
	// �����κ��� ���޹ޱ�

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

	ItemTrace();
}

void APlayerControllerMainMap::ItemTrace()
{
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}
	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector startLoc, dir;
	UGameplayStatics::DeprojectScreenToWorld(this, CrosshairLocation, startLoc, dir);
	GetWorld()->LineTraceSingleByChannel
	(
		itemHit,
		startLoc,
		startLoc + dir * 5000.f,
		ECC_ItemTrace
	);
	//DrawDebugLine(GetWorld(), startLoc, startLoc + dir * 5000.f, FColor::Blue, false, -1.f, 0U, 1.5f);
	if (itemHit.bBlockingHit)
	{
		TWeakObjectPtr<AItemBase> item = Cast<AItemBase>(itemHit.GetActor());
		if (item.IsValid())
		{
			lookingItem = item;
			GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Blue, TEXT("item"));
		}
		else lookingItem = nullptr;
	}
	else lookingItem = nullptr;
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
		const EPermanentItemType recentWeaponType = inventoryComponent->ArmRecentWeapon();
		if (myCharacter->RKeyPressed(recentWeaponType))
		{
			SendPlayerInputAction(EPlayerInputs::RKeyPressed, recentWeaponType);
		}
	}
}

void APlayerControllerMainMap::RKeyHold()
{
	const EPermanentItemType weaponType = inventoryComponent->GetCurrentWeaponType();
	if (weaponType != EPermanentItemType::NONE && myCharacter->RKeyHold(weaponType))
	{
		inventoryComponent->DisarmWeapon();
		SendPlayerInputAction(EPlayerInputs::RKeyHold, weaponType);
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
	// ������ ���� �ʿ� ���������� �˸�
	myCharacter->DZombieInRange.BindUFunction(this, FName("SendInRangeZombie"));
	myCharacter->DZombieOutRange.BindUFunction(this, FName("SendOutRangeZombie"));
	myCharacter->DZombieHitsMe.BindUFunction(this, FName("SendZombieHitsMe"));
	myCharacter->UpdatePlayerInfo();

	inventoryComponent->SetCharacter(myCharacter);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(defaultMappingContext, 1);
	}

	GetHUD<AHUDMainMap>()->StartHUD();
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
	myCharacter->AttachItemActor(itemActor);

	itemActor->GetItemObject()->SetOwnerController(this);
	itemActor->GetItemObject()->SetOwnerCharacter(myCharacter);
}

void APlayerControllerMainMap::NotifyToServerUnequipItem(const FString itemID, const FTile& addedPoint)
{
	clientSocket->UnequipItem(itemID, addedPoint.X, addedPoint.Y);
}

void APlayerControllerMainMap::UnequipItemAndAddToInventory(TWeakObjectPtr<AItemBase> itemActor, const FTile& addedPoint)
{
	inventoryComponent->UnequipItem(itemActor);
	AddItemToInventory(itemActor->GetItemObject(), addedPoint);
	myCharacter->DettachItemActor(itemActor);
}

void APlayerControllerMainMap::UnequipItem(TWeakObjectPtr<AItemBase> itemActor)
{
	inventoryComponent->UnequipItem(itemActor);
	myCharacter->DettachItemActor(itemActor);
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

void APlayerControllerMainMap::SendItemInfoToDrop(const FString itemID)
{
	check(clientSocket);
	clientSocket->SendItemInfoToDrop(itemID);
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
			// �÷��̾��� ��� �����ϴ� flag�� true��
			hittedCharacters.Add(TPair<int, bool>(player->GetPlayerNumber(), true));
		}
		else
		{
			TWeakObjectPtr<AZombieCharacter> zombie = Cast<AZombieCharacter>(hit.GetActor());
			if (zombie.IsValid())
			{
				// ������ ��� �����ϴ� flag�� false��
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

void APlayerControllerMainMap::SendItemUsing(const FString& itemID, const int usedAmount)
{
	check(clientSocket);
	clientSocket->SendItemUsing(itemID, usedAmount);
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

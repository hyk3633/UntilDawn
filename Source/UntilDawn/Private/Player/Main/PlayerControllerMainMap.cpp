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
#include "Tag/UntilDawnGameplayTags.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/AttributeSet/PlayerAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

APlayerControllerMainMap::APlayerControllerMainMap()
{
	asc = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	playerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("Player Attribute Set"));

	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> obj_DefaultContext(TEXT("/Game/_Assets/Inputs/IMC_DefaultsController.IMC_DefaultsController"));
	if (obj_DefaultContext.Succeeded()) defaultMappingContext = obj_DefaultContext.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_LeftClickPressed(TEXT("/Game/_Assets/Inputs/Actions/IA_LeftClickPressed.IA_LeftClickPressed"));
	if (obj_LeftClickPressed.Succeeded()) leftClickPressedAction = obj_LeftClickPressed.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_LeftClickTab(TEXT("/Game/_Assets/Inputs/Actions/IA_LeftClickTab.IA_LeftClickTab"));
	if (obj_LeftClickTab.Succeeded()) leftClickTabAction = obj_LeftClickTab.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_LeftClickHold(TEXT("/Game/_Assets/Inputs/Actions/IA_LeftClickHold.IA_LeftClickHold"));
	if (obj_LeftClickHold.Succeeded()) leftClickHoldAction = obj_LeftClickHold.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_LeftClickReleased(TEXT("/Game/_Assets/Inputs/Actions/IA_LeftClickReleased.IA_LeftClickReleased"));
	if (obj_LeftClickReleased.Succeeded()) leftClickReleasedAction = obj_LeftClickReleased.Object;

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

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_MouseSide(TEXT("/Game/_Assets/Inputs/Actions/IA_MouseSide.IA_MouseSide"));
	if (obj_MouseSide.Succeeded()) mouseSideAction = obj_MouseSide.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_ShiftPressed(TEXT("/Game/_Assets/Inputs/Actions/IA_ShiftPressed.IA_ShiftPressed"));
	if (obj_ShiftPressed.Succeeded()) shiftPressedAction = obj_ShiftPressed.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_ShiftReleased(TEXT("/Game/_Assets/Inputs/Actions/IA_ShiftReleased.IA_ShiftReleased"));
	if (obj_ShiftReleased.Succeeded()) shiftReleasedAction = obj_ShiftReleased.Object;
}

UAbilitySystemComponent* APlayerControllerMainMap::GetAbilitySystemComponent() const
{
	return asc;
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

	RecoverStamina(deltaTime);

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
		EnhancedInputComponent->BindAction(leftClickTabAction,		ETriggerEvent::Completed,	this, &APlayerControllerMainMap::WeaponInputPressed, EInputType::LeftClick_Tab);
		EnhancedInputComponent->BindAction(leftClickPressedAction,	ETriggerEvent::Triggered,	this, &APlayerControllerMainMap::WeaponInputPressed, EInputType::LeftClick_Pressed);
		EnhancedInputComponent->BindAction(leftClickHoldAction,		ETriggerEvent::Triggered,	this, &APlayerControllerMainMap::WeaponInputPressed, EInputType::LeftClick_Hold);
		EnhancedInputComponent->BindAction(leftClickReleasedAction,	ETriggerEvent::Completed,	this, &APlayerControllerMainMap::WeaponInputPressed, EInputType::LeftClick_Released);

		EnhancedInputComponent->BindAction(rightClickAction,		ETriggerEvent::Triggered,	this, &APlayerControllerMainMap::WeaponInputPressed, EInputType::RightClick_Pressed);
		EnhancedInputComponent->BindAction(rightClickAction,		ETriggerEvent::Completed,	this, &APlayerControllerMainMap::WeaponInputPressed, EInputType::RightClick_Released);

		EnhancedInputComponent->BindAction(mouseSideAction,			ETriggerEvent::Completed,	this, &APlayerControllerMainMap::WeaponInputPressed, EInputType::MouseSide_Pressed);

		EnhancedInputComponent->BindAction(rKeyAction,				ETriggerEvent::Completed,	this, &APlayerControllerMainMap::NormalInputPressed, EInputType::R_Pressed);
		EnhancedInputComponent->BindAction(rKeyHoldAction,			ETriggerEvent::Triggered,	this, &APlayerControllerMainMap::NormalInputPressed, EInputType::R_Hold);
		EnhancedInputComponent->BindAction(rKeyHoldAction,			ETriggerEvent::Triggered,	this, &APlayerControllerMainMap::NormalInputPressed, EInputType::R_Hold);
		EnhancedInputComponent->BindAction(shiftPressedAction,		ETriggerEvent::Triggered,	this, &APlayerControllerMainMap::NormalInputPressed, EInputType::Shift_Pressed);
		EnhancedInputComponent->BindAction(shiftReleasedAction,		ETriggerEvent::Triggered,	this, &APlayerControllerMainMap::NormalInputPressed, EInputType::Shift_Released);

		EnhancedInputComponent->BindAction(eKeyAction,				ETriggerEvent::Completed, this, &APlayerControllerMainMap::EKeyPressed);
		EnhancedInputComponent->BindAction(iKeyAction,				ETriggerEvent::Completed, this, &APlayerControllerMainMap::IKeyPressed);

		EnhancedInputComponent->BindAction(hKeyAction,				ETriggerEvent::Completed, this, &APlayerControllerMainMap::HKeyPressed);
		EnhancedInputComponent->BindAction(wheelAction,				ETriggerEvent::Completed, this, &APlayerControllerMainMap::WheelRolled);
	}
}

void APlayerControllerMainMap::WeaponInputPressed(const EInputType inputType)
{
	myCharacter->TryActivateWeaponAbility(inputType);
}

void APlayerControllerMainMap::NormalInputPressed(const EInputType inputType)
{
	myCharacter->TryActivateInputAbility(inputType);
}

void APlayerControllerMainMap::ArmWeapon()
{
	const EWeaponType currentWeaponType = inventoryComponent->GetCurrentWeaponType();
	if (currentWeaponType == EWeaponType::NONE)
	{
		auto itemActor = inventoryComponent->ArmRecentWeapon();
		if (itemActor.IsValid() && myCharacter->ArmWeapon(itemActor))
		{
			onWeaponArmed.ExecuteIfBound(itemActor->GetItemObject().Get());
			clientSocket->ArmWeapon(itemActor->GetItemID());
		}
	}
}

void APlayerControllerMainMap::DisarmWeapon()
{
	const EWeaponType weaponType = inventoryComponent->GetCurrentWeaponType();
	if (weaponType != EWeaponType::NONE && myCharacter->DisarmWeapon())
	{
		inventoryComponent->DisarmWeapon();
		clientSocket->DisarmWeapon();
		onWeaponArmed.ExecuteIfBound(nullptr);
	}
}

void APlayerControllerMainMap::EKeyPressed()
{
	check(myCharacter);
	if (myCharacter->IsWrestling())
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
			onWeaponArmed.ExecuteIfBound(changedWeaponActor->GetItemObject().Get());
			myCharacter->ChangeWeapon(changedWeaponActor);

			FGameplayEventData payloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(myCharacter, UD_EVENT_CHARACTER_CHANGEWEAPON, payloadData);

			clientSocket->ChangeWeapon(changedWeaponActor->GetItemID());
		}
	}
}

void APlayerControllerMainMap::WrestlingStart()
{
	DWrestlingStart.ExecuteIfBound();
}

void APlayerControllerMainMap::CancelWrestling()
{
	DWrestlingEnd.ExecuteIfBound();
}

void APlayerControllerMainMap::SuccessToBlocking()
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(myCharacter, UD_EVENT_WRESTLING_BLOCKED, FGameplayEventData());
	SendPlayerBlockingResult(true);
}

void APlayerControllerMainMap::FailedToBlocking()
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(myCharacter, UD_EVENT_WRESTLING_BITED, FGameplayEventData());
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

void APlayerControllerMainMap::SendInRangeZombie(int zombieNumber)
{
	clientSocket->SendInRangeZombie(zombieNumber);
}

void APlayerControllerMainMap::SendOutRangeZombie(int zombieNumber)
{
	clientSocket->SendOutRangeZombie(zombieNumber);
}

void APlayerControllerMainMap::SendZombieHitsMe(const int zombieNumber, const bool bResult, FHitResult& hitResult)
{
	FHitInfo hitInfo{ zombieNumber, false , hitResult.ImpactPoint, hitResult.ImpactNormal.Rotation() };
	clientSocket->SendZombieHitsMe(zombieNumber, bResult, hitInfo);
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
		onWeaponArmed.ExecuteIfBound(nullptr);
	}
	myCharacter->UnEquipItem(itemActor);
	inventoryComponent->UnequipItem(itemActor);
	AddItemToInventory(itemActor->GetItemObject(), addedPoint);
}

void APlayerControllerMainMap::UnequipItem(TWeakObjectPtr<AItemBase> itemActor)
{
	if (myCharacter->GetArmedWeapon() == itemActor)
	{
		onWeaponArmed.ExecuteIfBound(nullptr);
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

void APlayerControllerMainMap::SendHittedCharacters(TArray<FHitResult>& hits, const FString& itemID)
{
	check(clientSocket);
	TArray<FHitInfo> hittedCharacters;
	for (auto& hit : hits)
	{
		if (hit.bBlockingHit == false)
			continue;
		TWeakObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(hit.GetActor());
		if (player.IsValid())
		{
			hittedCharacters.Add(FHitInfo{ player->GetPlayerNumber(), true , hit.ImpactPoint, hit.ImpactNormal.Rotation()});
		}
		else
		{
			TWeakObjectPtr<AZombieCharacter> zombie = Cast<AZombieCharacter>(hit.GetActor());
			if (zombie.IsValid())
			{
				hittedCharacters.Add(FHitInfo{ zombie->GetNumber(), false , hit.ImpactPoint, hit.ImpactNormal.Rotation() });
			}
		}
	}
	if (hittedCharacters.Num())
	{
		clientSocket->SendHittedCharacters(hittedCharacters, itemID);
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
	bAttacking = true;
}

void APlayerControllerMainMap::Attack()
{
	inventoryComponent->Attack();
}

void APlayerControllerMainMap::EndAttack()
{
	bAttacking = false;
}

bool APlayerControllerMainMap::IsAttacking()
{
	return bAttacking;
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

void APlayerControllerMainMap::SendHitResult(TArray<FHitResult>& hits, const FString& itemID)
{
	TArray<TPair<int, int>> kickedCharacters;
	for (auto& hit : hits)
	{
		if (hit.bBlockingHit == false)
			continue;
		TWeakObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(hit.GetActor());
		if (player.IsValid())
		{
			kickedCharacters.Add({ player->GetPlayerNumber(), true });
		}
		else
		{
			TWeakObjectPtr<AZombieCharacter> zombie = Cast<AZombieCharacter>(hit.GetActor());
			if (zombie.IsValid())
			{
				kickedCharacters.Add({ zombie->GetNumber(), false });
			}
		}
	}
	if (kickedCharacters.Num())
	{
		clientSocket->SendKickedCharacters(kickedCharacters);
	}
	
}

void APlayerControllerMainMap::SendActivatedWeaponAbility(const int32 inputType)
{
	clientSocket->SendActivateWeaponAbility(inputType);
}

void APlayerControllerMainMap::SetStamina(const int newStamina)
{
	maxStamina = newStamina;
	playerAttributeSet->InitStamina(newStamina);
}

void APlayerControllerMainMap::StaminaChanged(const float newStamina)
{
	ensure(maxStamina > 0);
	DStaminaChanged.ExecuteIfBound(newStamina / maxStamina);
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

void APlayerControllerMainMap::RecoverStamina(float deltaTime)
{
	if (asc->HasMatchingGameplayTag(UD_CHARACTER_STATE_BLOCKSTAMINARECOVERY) == false)
	{
		if (playerAttributeSet->GetStamina() < maxStamina)
		{
			playerAttributeSet->RecoverStamina(25 * deltaTime);
			StaminaChanged(playerAttributeSet->GetStamina());
		}
	}
}

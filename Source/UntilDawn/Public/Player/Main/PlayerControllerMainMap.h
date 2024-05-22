// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Structs/CharacterInfo.h"
#include "Structs/Tile.h"
#include "Enums/PlayerInputs.h"
#include "Enums/PermanentItemType.h"
#include "PlayerControllerMainMap.generated.h"

/**
 * 
 */

class ClientSocket;
class APlayerCharacter;
class UInputMappingContext;
class UInputAction;
class UItemObject;
class AItemBase;
class UInventoryComponent;

DECLARE_DELEGATE(DelegatePlayerDead);
DECLARE_DELEGATE(DelegateWrestlingStart);
DECLARE_DELEGATE(DelegateEKeyPressed);
DECLARE_DELEGATE(DelegateIKeyPressed);
DECLARE_DELEGATE_TwoParams(DelegateEquipItem, UItemObject* itemObj, const int boxNumber);
DECLARE_DELEGATE_OneParam(DelegateHealthChanged, float healthPercentage);

UCLASS()
class UNTILDAWN_API APlayerControllerMainMap : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerMainMap();

	DelegatePlayerDead DPlayerDead;
	DelegateWrestlingStart DWrestlingStart;
	DelegateEKeyPressed DEKeyPressed;
	DelegateIKeyPressed DIKeyPressed;
	DelegateEquipItem DEquipItem;
	DelegateHealthChanged DHealthChanged;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float deltaTime) override;

protected:

	void Trace();

	void GetTraceLocationAndDirection(FVector& location, FVector& direction);

	void CharacterTrace(const FVector& location, const FVector& direction);

	void ItemTrace(const FVector& location, const FVector& direction);

	virtual void SetupInputComponent() override;

	void LeftClick();

	void LeftClickHold();

	void LeftClickEnd();

	void RightClick();

	void RightClickEnd();

	void RKeyPressed();

	void RKeyHold();

	void EKeyPressed();

	void IKeyPressed();

	void HKeyPressed();

public:

	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return inventoryComponent; }

	void WrestlingStart();

	void WrestlingEnd(const bool wrestlingResult);

	void SuccessToBlocking();

	void FailedToBlocking();

	virtual void OnPossess(APawn* pawn) override;

	void SendPlayerInputAction(const EPlayerInputs inputType, const EPermanentItemType weaponType);

	UFUNCTION()
	void SendInRangeZombie(int zombieNumber);

	UFUNCTION()
	void SendOutRangeZombie(int zombieNumber);

	UFUNCTION()
	void SendZombieHitsMe(int zombieNumber, bool bResult);

	void SendPlayerBlockingResult(const bool isSuccessToBlocking);

	void SendPickedItemInfo(const FString itemID);

	void AddItemToInventory(TWeakObjectPtr<UItemObject> itemObj, const FTile& addedPoint);

	void NotifyToServerUpdateItemGridPoint(const FString itemID, const int xPoint, const int yPoint, const bool isRotated);

	void UpdateItemInventoryGrid(TWeakObjectPtr<UItemObject> itemObj, const int xIndex, const int yIndex);

	void SendItemInfoToEquip(const FString itemID, const int boxNumber);

	void EquipItem(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor);

	void NotifyToServerUnequipItem(const FString itemID, const FTile& addedPoint);

	void UnequipItemAndAddToInventory(TWeakObjectPtr<AItemBase> itemActor, const FTile& addedPoint);

	void UnequipItem(TWeakObjectPtr<AItemBase> itemActor);

	void RestoreInventoryUI(TWeakObjectPtr<UItemObject> itemObj);

	void RestoreEquipmentUI(TWeakObjectPtr<UItemObject> itemObj);

	void DropEquippedItem(const FString itemID);

	void SendItemInfoToDrop(const FString itemID);

	void SendHittedCharacters(TArray<FHitResult>& hits);

	void PlayerDead();

	void StartAttack();

	void ReplicateProjectile(const FVector& location, const FRotator& rotation);

	TWeakObjectPtr<UItemObject> GetItemObjectOfType(const EItemMainType itemType);
	
	void SendItemUsing(const FString& itemID, const int usedAmount);

	void ItemExhausted(TWeakObjectPtr<UItemObject> itemObj);

	void UpdateHealth(const float health);

protected:

	void SynchronizePlayerInfo();

	void respawnRequestAfterDelay();

protected:

	ClientSocket* clientSocket;

	UPROPERTY()
	UInventoryComponent* inventoryComponent;

	UPROPERTY()
	APlayerCharacter* myCharacter;

	FTimerHandle SynchronizeTimer;

	FTimerHandle respawnRequestTimer;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* defaultMappingContext;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* leftClickAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* leftClickHoldAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* rightClickAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* rKeyAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* rKeyHoldAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* eKeyAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* iKeyAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* hKeyAction;

	FHitResult itemHit, characterHit;

	TWeakObjectPtr<AItemBase> lookingItem;

};

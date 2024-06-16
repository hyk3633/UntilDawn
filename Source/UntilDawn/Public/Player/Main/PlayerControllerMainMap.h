// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Structs/CharacterInfo.h"
#include "Structs/Tile.h"
#include "Structs/HitInfo.h"
#include "Enums/PlayerInputs.h"
#include "Enums/WeaponType.h"
#include "Enums/InputType.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
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
class UAbilitySystemComponent;
class UPlayerAttributeSet;
class USoundCue;

DECLARE_DELEGATE(DelegatePlayerDead);
DECLARE_DELEGATE(DelegateWrestlingStart);
DECLARE_DELEGATE(DelegateWrestlingEnd);
DECLARE_DELEGATE(DelegateEKeyPressed);
DECLARE_DELEGATE(DelegateIKeyPressed);
DECLARE_DELEGATE_TwoParams(DelegateEquipItem, UItemObject* itemObj, const int boxNumber);
DECLARE_DELEGATE_OneParam(DelegateHealthChanged, float healthPercentage);
DECLARE_DELEGATE_OneParam(DelegateStaminaChanged, float staminaPercentage);
DECLARE_DELEGATE_OneParam(OnWeaponArmed, UItemObject* itemObj);


UCLASS()
class UNTILDAWN_API APlayerControllerMainMap : public APlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	APlayerControllerMainMap();

	DelegatePlayerDead DPlayerDead;
	DelegateWrestlingStart DWrestlingStart;
	DelegateWrestlingEnd DWrestlingEnd;
	DelegateEKeyPressed DEKeyPressed;
	DelegateIKeyPressed DIKeyPressed;
	DelegateEquipItem DEquipItem;
	DelegateHealthChanged DHealthChanged;
	DelegateStaminaChanged DStaminaChanged;
	OnWeaponArmed onWeaponArmed;

protected:

	virtual void BeginPlay() override;

public:

	virtual void OnPossess(APawn* pawn) override;

protected:

	void SynchronizePlayerInfo();

public:

	virtual void Tick(float deltaTime) override;

protected:

	void RecoverStamina(float deltaTime);

	void Trace();

	void GetTraceLocationAndDirection(FVector& location, FVector& direction);

	void CharacterTrace(const FVector& location, const FVector& direction);

	void ItemTrace(const FVector& location, const FVector& direction);

	/* 입력 */

	virtual void SetupInputComponent() override;

	void WeaponInputPressed(const EInputType inputType);

	void NormalInputPressed(const EInputType inputType);

	void EKeyPressed();

	void PlayItemPickUpSound();

	void SendPickedItemInfo(const FString itemID);

	void IKeyPressed();

	void HKeyPressed();

	void WheelRolled();

public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	
	/* 아이템 */

	void ArmWeapon();

	void DisarmWeapon();

	void EquipItem(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor);

	void UnequipItem(TWeakObjectPtr<AItemBase> itemActor);

	void SendItemInfoToEquip(const FString itemID, const int boxNumber);

	void NotifyToServerUnequipItem(const FString itemID, const FTile& addedPoint);

	TWeakObjectPtr<UItemObject> GetItemObjectOfType(const EItemMainType itemType);

	void SendItemUsing(const FString& itemID, const int consumedAmount);

	void ItemExhausted(TWeakObjectPtr<UItemObject> itemObj);

	/* 인벤토리 */

	void AddItemToInventory(TWeakObjectPtr<UItemObject> itemObj, const FTile& addedPoint);

	void UpdateItemInventoryGrid(TWeakObjectPtr<UItemObject> itemObj, const int xIndex, const int yIndex);

	void NotifyToServerUpdateItemGridPoint(const FString itemID, const int xPoint, const int yPoint, const bool isRotated);

	void UnequipItemAndAddToInventory(TWeakObjectPtr<AItemBase> itemActor, const FTile& addedPoint);

	void RestoreInventoryUI(TWeakObjectPtr<UItemObject> itemObj);

	void RestoreEquipmentUI(TWeakObjectPtr<UItemObject> itemObj);

	void DropEquippedItem(const FString itemID);

	void DropInventoryItem(const FString itemID);

	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return inventoryComponent; }

	/* 공격 */

	void StartAttack();

	void Attack();

	void EndAttack();

	FORCEINLINE bool IsAttacking() { return bAttacking; }

	void SendActivatedWeaponAbility(const int32 inputType);

	void SendHittedCharacter(const FHitResult& hit, const FString& itemID);

	void ReplicateProjectile(const FVector& location, const FRotator& rotation);

	void SendKickResult(FHitResult& hit);

protected:

	/* 좀비 상호작용 */

	UFUNCTION()
	void SendInRangeZombie(int zombieNumber);

	UFUNCTION()
	void SendOutRangeZombie(int zombieNumber);

public:

	void SendZombieHitsMe(const int zombieNumber, const bool bResult, FHitResult& hitResult);

	void SendPlayerBlockingResult(const bool isSuccessToBlocking);

	void WrestlingStart();

	void CancelWrestling();

	void SuccessToBlocking();

	void FailedToBlocking();

public:

	/* 죽음 */

	void PlayerDead();

protected:

	void RespawnRequestAfterDelay();

public:

	/* 상태 동기화 */

	void UpdateHealth(const float health);

	void SetRowColumn(const int r, const int c);

	void SetStamina(const int newStamina);

	void StaminaChanged(const float newStamina);

protected:

	ClientSocket* clientSocket;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> asc;

	UPROPERTY()
	TObjectPtr<UPlayerAttributeSet> playerAttributeSet;

	UPROPERTY()
	UInventoryComponent* inventoryComponent;

	UPROPERTY()
	APlayerCharacter* myCharacter;

	FTimerHandle SynchronizeTimer;

	FTimerHandle respawnRequestTimer;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* defaultMappingContext;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* leftClickTabAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* leftClickPressedAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* leftClickHoldAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* leftClickReleasedAction;

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

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* wheelAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* mouseSideAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* shiftPressedAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* shiftReleasedAction;

	UPROPERTY()
	USoundCue* itemPickUpSound;

	FHitResult itemHit, characterHit;

	TWeakObjectPtr<AItemBase> lookingItem;

	bool bAttacking;

	float maxStamina;

};

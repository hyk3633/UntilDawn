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

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

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

	virtual void Tick(float deltaTime) override;

protected:

	void Trace();

	void GetTraceLocationAndDirection(FVector& location, FVector& direction);

	void CharacterTrace(const FVector& location, const FVector& direction);

	void ItemTrace(const FVector& location, const FVector& direction);

	virtual void SetupInputComponent() override;

	void WeaponInputPressed(const EInputType inputType);

	void NormalInputPressed(const EInputType inputType);

public:

	void ArmWeapon();

	void DisarmWeapon();

protected:

	void EKeyPressed();

	void IKeyPressed();

	void HKeyPressed();

	void WheelRolled();

public:

	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return inventoryComponent; }

	void WrestlingStart();

	void CancelWrestling();

	void SuccessToBlocking();

	void FailedToBlocking();

	virtual void OnPossess(APawn* pawn) override;

	UFUNCTION()
	void SendInRangeZombie(int zombieNumber);

	UFUNCTION()
	void SendOutRangeZombie(int zombieNumber);

	void SendZombieHitsMe(const int zombieNumber, const bool bResult, FHitResult& hitResult);

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

	void DropInventoryItem(const FString itemID);

	void SendHittedCharacters(TArray<FHitResult>& hits, const FString& itemID);

	void PlayerDead();

	void StartAttack();

	void Attack();

	void EndAttack();

	bool IsAttacking();

	void ReplicateProjectile(const FVector& location, const FRotator& rotation);

	TWeakObjectPtr<UItemObject> GetItemObjectOfType(const EItemMainType itemType);
	
	void SendItemUsing(const FString& itemID, const int consumedAmount);

	void ItemExhausted(TWeakObjectPtr<UItemObject> itemObj);

	void UpdateHealth(const float health);

	void SetRowColumn(const int r, const int c);

	void SendHitResult(TArray<FHitResult>& hits, const FString& itemID);

	void SendActivatedWeaponAbility(const int32 inputType);

	void SetStamina(const int newStamina);

	void StaminaChanged(const float newStamina);

protected:

	void SynchronizePlayerInfo();

	void respawnRequestAfterDelay();

	void RecoverStamina(float deltaTime);

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

	FHitResult itemHit, characterHit;

	TWeakObjectPtr<AItemBase> lookingItem;

	bool bAttacking;

	float maxStamina;

};

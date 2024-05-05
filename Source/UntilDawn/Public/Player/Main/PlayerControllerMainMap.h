// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Structs/CharacterInfo.h"
#include "Enums/PlayerInputs.h"
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

DECLARE_DELEGATE(DelegatePlayerDead);
DECLARE_DELEGATE(DelegateWrestlingStart);
DECLARE_DELEGATE(DelegateEKeyPressed);
DECLARE_DELEGATE(DelegateIKeyPressed);
DECLARE_DELEGATE_TwoParams(DelegateItemEquip, UItemObject* itemObj, const int boxNumber);

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
	DelegateItemEquip DItemEquip;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float deltaTime) override;

protected:

	void ItemTrace();

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

	void WrestlingStart();

	void WrestlingEnd(const bool wrestlingResult);

	void SuccessToBlocking();

	void FailedToBlocking();

	virtual void OnPossess(APawn* pawn) override;

	void SendPlayerInputAction(const EPlayerInputs inputType);

	UFUNCTION()
	void SendInRangeZombie(int zombieNumber);

	UFUNCTION()
	void SendOutRangeZombie(int zombieNumber);

	UFUNCTION()
	void SendZombieHitsMe(int zombieNumber, bool bResult);

	void SendPlayerBlockingResult(const bool isSuccessToBlocking);

	void SendPickedItemInfo(const int itemID);

	void UpdateItemGridPoint(const int itemID, const int xPoint, const int yPoint, const bool isRotated);

	void SendItemInfoToEquip(const int itemID, const int boxNumber);

	void ProcessItemEquipInUI(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor);

	void RestoreInventoryUI(TWeakObjectPtr<UItemObject> itemObj);

	void SendItemInfoToDrop(const int itemID);

	void SendHittedCharacterInfo(TArray<FHitResult>& hits);

	void PlayerDead();

protected:

	void SynchronizePlayerInfo();

	void respawnRequestAfterDelay();

protected:

	ClientSocket* clientSocket;

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

	FHitResult itemHit;

	TWeakObjectPtr<AItemBase> lookingItem;

};

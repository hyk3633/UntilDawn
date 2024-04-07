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

DECLARE_DELEGATE(DelegatePlayerDead);
DECLARE_DELEGATE(DelegateWrestlingStart);
DECLARE_DELEGATE(DelegateEKeyPressed);

class ClientSocket;
class APlayerCharacter;
class UInputMappingContext;
class UInputAction;
class AItemMeleeWeapon;

UCLASS()
class UNTILDAWN_API APlayerControllerMainMap : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerMainMap();

	DelegatePlayerDead DPlayerDead;
	DelegateWrestlingStart DWrestlingStart;
	DelegateEKeyPressed DEKeyPressed;

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

public:

	void WrestlingStart();

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

	void SendPickedItemInfo(const int itemNumber);

	UFUNCTION()
	void SendHitPlayerInfo(const int playerNumber);

	UFUNCTION()
	void SendHitZombieInfo(const int zombieNumber);

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

	FHitResult itemHit;

	TWeakObjectPtr<AItemMeleeWeapon> lookingItem;

};

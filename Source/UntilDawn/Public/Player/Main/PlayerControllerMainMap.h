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

class ClientSocket;
class APlayerCharacter;

UCLASS()
class UNTILDAWN_API APlayerControllerMainMap : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerMainMap();

	virtual void BeginPlay() override;

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

	void SendHitPlayerInfo(const int playerNumber);

	void SendHitZombieInfo(const int zombieNumber);

	void PlayerDead();

	DelegatePlayerDead DPlayerDead;

protected:

	void SynchronizePlayerInfo();

	void respawnRequestAfterDelay();

protected:

	ClientSocket* clientSocket;

	UPROPERTY()
	APlayerCharacter* myCharacter;

	FTimerHandle SynchronizeTimer;

	FTimerHandle respawnRequestTimer;

};

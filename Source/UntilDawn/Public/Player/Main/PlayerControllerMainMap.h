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

UCLASS()
class UNTILDAWN_API APlayerControllerMainMap : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerMainMap();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* pawn) override;

	void SendPlayerInputAction(const EPlayerInputs inputType);

	void SendPlayerBlockingResult(const bool isSuccessToBlocking);

	void SendPickedItemInfo(const int itemNumber);

	void SendHitPlayerInfo(const int playerNumber);

	void SendHitZombieInfo(const int zombieNumber);

protected:

	void SynchronizePlayerInfo();

protected:

	ClientSocket* clientSocket;

	UPROPERTY()
	APlayerCharacter* myCharacter;

	FTimerHandle SynchronizeTimer;

};

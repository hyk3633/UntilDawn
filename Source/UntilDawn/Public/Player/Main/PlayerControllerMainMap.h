// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Structs/PlayerInfo.h"
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

protected:

	void UpdatePlayerInfo();

	void SynchronizePlayerInfo();

protected:

	ClientSocket* clientSocket;

	UPROPERTY()
	APlayerCharacter* myCharacter;

	FTimerHandle SynchronizeTimer;

	PlayerInfo myInfo;
};

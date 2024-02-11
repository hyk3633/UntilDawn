// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structs/PlayerInfo.h"
#include "GameModeMainMap.generated.h"

/**
 * 
 */

class ClientSocket;
class APlayerCharacter;
class PlayerInfoSetEx;

UCLASS()
class UNTILDAWN_API AGameModeMainMap : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGameModeMainMap();

	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	void ReceiveNewPlayerInfo(PlayerInfoSetEx* newPlayerInfoSet);

	void ReceiveOtherPlayersInfo(PlayerInfoSet* synchPlayerInfoSet);

protected:

	void SpawnNewPlayerCharacter();

	void SynchronizeOtherPlayersInfo();

private:
	
	ClientSocket* clientSocket;

	TSubclassOf<APlayerCharacter> defaultPawnClass;

	TMap<int, APlayerCharacter*> playerCharacterMap;

	PlayerInfoSetEx* playerInfoSetEx;

	PlayerInfoSet* playerInfoSet;

	int myNumber;

};

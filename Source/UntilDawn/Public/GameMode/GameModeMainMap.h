// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structs/PlayerInfo.h"
#include "GameModeMainMap.generated.h"

/**
 * 
 */

class UActorSpawner;
class UActorPooler;
class ClientSocket;
class APlayerCharacter;
class PlayerInfoSetEx;

UCLASS()
class UNTILDAWN_API AGameModeMainMap : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGameModeMainMap();

protected:

	virtual void BeginPlay() override;

	void DestroyPlayer();

	void SpawnNewPlayerCharacter();

	void SynchronizeOtherPlayersInfo();

public:

	virtual void Tick(float deltaTime) override;

	void ReceiveNewPlayerInfo(PlayerInfoSetEx* newPlayerInfoSet);

	void ReceiveOtherPlayersInfo(PlayerInfoSet* synchPlayerInfoSet);

	void ReceiveDisconnectedPlayerInfo(const int playerNumber, const FString playerID);

private:

	UPROPERTY()
	UActorSpawner* actorSpawner;
	
	ClientSocket* clientSocket;

	TQueue<int> playerToDelete;

	TMap<int, APlayerCharacter*> playerCharacterMap;

	PlayerInfoSetEx* playerInfoSetEx;

	PlayerInfoSet* playerInfoSet;

	int myNumber;

};

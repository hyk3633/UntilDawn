// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UntilDawnGameInstance.generated.h"

/**
 * 
 */

class ClientSocket;

UCLASS()
class UNTILDAWN_API UUntilDawnGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	ClientSocket* GetSocket();

	void EndConnection();

	FORCEINLINE void SetPlayerNumber(int number) { playerNumber = number; }
	FORCEINLINE int GetPlayerNumber() const { return playerNumber; }

protected:

	virtual void Shutdown() override;
	
private:

	ClientSocket* socket;

	int playerNumber;
};

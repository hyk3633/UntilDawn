// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <sstream>
#include <queue>
#include <vector>
#include <string>
#include "GameModeLoginMap.generated.h"

/**
 * 
 */

class ClientSocket;
class APlayerControllerLoginMap;

UCLASS()
class UNTILDAWN_API AGameModeLoginMap : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AGameModeLoginMap();

	virtual void BeginPlay() override;

protected:

	virtual void Tick(float deltaTime) override;

	void ProcessPacket();

	void ReceiveSignUpResult(std::stringstream& recvStream);

	void ReceiveLoginResult(std::stringstream& recvStream);

private:

	ClientSocket* clientSocket;

	UPROPERTY()
	APlayerControllerLoginMap* loginController;

	bool isConnected;

	std::vector<void (AGameModeLoginMap::*)(std::stringstream&)> packetCallbacks;

};

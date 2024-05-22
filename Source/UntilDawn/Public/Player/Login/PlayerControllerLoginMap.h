// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <sstream>
#include <queue>
#include <vector>
#include <string>
#include "PlayerControllerLoginMap.generated.h"
/**
 * 
 */

class ClientSocket;
class AHUDLoginMap;

UCLASS()
class UNTILDAWN_API APlayerControllerLoginMap : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerLoginMap();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:

	void ProcessPacket();

	UFUNCTION()
	void ReceiveAccountInfo(const FText& id, const FText& pw, const bool isLogin);

	void SetSignUpMessageText(std::stringstream& recvStream);

	void SetLoginMessageTextAndLoginToMap(std::stringstream& recvStream);

	void StartLevelTransition();
	
private:

	ClientSocket* clientSocket;

	UPROPERTY()
	AHUDLoginMap* loginMapHUD;

	bool isConnected;

	FTimerHandle levelTransitionTimer;

	std::vector<void (APlayerControllerLoginMap::*)(std::stringstream&)> packetCallbacks;

};

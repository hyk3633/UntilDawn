// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerLoginMap.generated.h"
/**
 * 
 */

class AHUDLoginMap;
class ClientSocket;

UCLASS()
class UNTILDAWN_API APlayerControllerLoginMap : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerLoginMap();

	virtual void BeginPlay() override;

	void SetSocket(ClientSocket* socket);

	void SetSignUpMessageText(const bool isGranted);

	void SetLoginMessageTextAndLoginToMap(const bool isGranted, FString playerID = TEXT(""), const int playerNumber = -1);

protected:

	UFUNCTION()
	void ReceiveAccountInfo(const FText& id, const FText& pw, const bool isLogin);

	void StartLevelTransition();
	
private:

	ClientSocket* clientSocket;

	UPROPERTY()
	AHUDLoginMap* loginMapHUD;

	FTimerHandle levelTransitionTimer;

};

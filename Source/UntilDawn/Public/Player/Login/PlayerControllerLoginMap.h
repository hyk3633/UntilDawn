// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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

	void ReceiveSignUpRequestResult(const bool);

	void ReceiveLoginRequestResult(const bool);

protected:

	UFUNCTION()
	void ReceiveAccountInfo(const FText& id, const FText& pw, const bool isLogin);

	void SetSignUpMessageText();

	void SetLoginMessageText();
	
private:

	ClientSocket* clientSocket;

	UPROPERTY()
	AHUDLoginMap* loginMapHUD;

	bool isConnected;

	bool bSetSignUpMessageText;
	bool bSetLoginMessageText;
	bool bFlag;

};

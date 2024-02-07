// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerMainMap.generated.h"

/**
 * 
 */

class ClientSocket;

UCLASS()
class UNTILDAWN_API APlayerControllerMainMap : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerMainMap();

	virtual void BeginPlay() override;

protected:

	ClientSocket* clientSocket;

};

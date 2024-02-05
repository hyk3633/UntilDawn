// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerLoginMap.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API APlayerControllerLoginMap : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerLoginMap();

	virtual void BeginPlay() override;
	
};

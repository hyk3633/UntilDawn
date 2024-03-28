// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeLoginMap.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API AGameModeLoginMap : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AGameModeLoginMap();

	virtual void BeginPlay() override;

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

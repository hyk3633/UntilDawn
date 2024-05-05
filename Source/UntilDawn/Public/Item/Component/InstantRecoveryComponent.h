// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InstantRecoveryComponent.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UInstantRecoveryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInstantRecoveryComponent();

protected:
	
	virtual void BeginPlay() override;

public:	

	void InstantHealthRecovery(TWeakObjectPtr<APlayerCharacter> playerCharacter, const float recoveryAmount);
		
};

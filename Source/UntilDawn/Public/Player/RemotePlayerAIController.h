// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AbilitySystemInterface.h"
#include "RemotePlayerAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API ARemotePlayerAIController : public AAIController, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ARemotePlayerAIController();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> asc;

};

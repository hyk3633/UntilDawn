// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AbilitySystemInterface.h"
#include "RemotePlayerAIController.generated.h"

/**
 * 
 */

class UPlayerAttributeSet;

UCLASS()
class UNTILDAWN_API ARemotePlayerAIController : public AAIController, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ARemotePlayerAIController();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnPossess(APawn* InPawn) override;
	
private:

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> asc;

	UPROPERTY()
	TObjectPtr<UPlayerAttributeSet> playerAttributeSet;

};

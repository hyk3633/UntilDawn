// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_HitCheck.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UGA_HitCheck : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_HitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

private:

	UPROPERTY(EditAnywhere, Category = "GAS | Option")
	FName socketName;

	UPROPERTY(EditAnywhere, Category = "GAS | Option")
	float sphereRadius;

	UPROPERTY(EditAnywhere, Category = "GAS | Option")
	FGameplayTag triggerGameplayTag;
	
};

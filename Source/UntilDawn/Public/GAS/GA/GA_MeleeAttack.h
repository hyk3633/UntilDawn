// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_MeleeAttack.generated.h"

/**
 * 
 */

class UAbilityTask_PlayMontageAndWait;
class UAnimMontage;
class APlayerCharacter;
class APlayerControllerMainMap;

UCLASS()
class UNTILDAWN_API UGA_MeleeAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_MeleeAttack();

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	FName GetNextSection();

	void StartComboTimer();

	void CheckComboInput();

	bool CheckStamina();

	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	void SendAbilityActivationToController();

private:

	UPROPERTY(EditAnywhere, Category = "GAS | Animation")
	UAnimMontage* meleeAttackMontage;

	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* playMeleeAttackTask;

	UPROPERTY(EditAnywhere, Category = "GAS | Option")
	TArray<float> comboActivationSeconds;

	TWeakObjectPtr<APlayerCharacter> character;

	TWeakObjectPtr<APlayerControllerMainMap> controller;

	uint8 currentCombo = 0;

	FTimerHandle comboTimer;

	bool hasNextComboInput = false;

};

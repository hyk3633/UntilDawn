// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../../Enums/WeaponType.h"
#include "GA_JustPlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UGA_JustPlayMontage : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_JustPlayMontage();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

private:

	UPROPERTY(EditAnywhere, Category = "GAS | Animations")
	TMap<EWeaponType, UAnimMontage*> montages;

	UPROPERTY(EditAnywhere, Category = "GAS | Option")
	bool isPlayer = true;

	UPROPERTY(EditAnywhere, Category = "GAS | Option", meta = (EditCondition = "isPlayer"))
	bool bNeedWeapon;

	UPROPERTY(EditAnywhere, Category = "GAS | Option", meta = (EditCondition = "isPlayer"))
	bool bShouldReplicate;

	UPROPERTY(EditAnywhere, Category = "GAS | Option", meta = (EditCondition = "isPlayer"))
	bool bCanMoving = true;

};

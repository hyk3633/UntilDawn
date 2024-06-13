// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../../Enums/WeaponType.h"
#include "GA_HitReaction.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UGA_HitReaction : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_HitReaction();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();
	
	FName GetHitReactDirection(const FVector& impactPoint);

	void InvokeGameplayCue(EWeaponType weaponType, const FHitResult& hitResult);

private:

	UPROPERTY(EditAnywhere, Category = "GAS | Animations")
	UAnimMontage* hitReactionMontage;

	UPROPERTY(EditAnywhere, Category = GAS, meta = (Categories = GameplayCue))
	TMap<EWeaponType, FGameplayTag> gameplayCueTags;

};

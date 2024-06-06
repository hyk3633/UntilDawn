// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../../Enums/WeaponType.h"
#include "GA_BowShoot.generated.h"

/**
 * 
 */

class APlayerControllerMainMap;
class APlayerCharacter;
class UAbilityTask_PlayMontageAndWait;
class AProjectileBase;
class UItemPermanent;

UENUM(BlueprintType)
enum class EBowState : uint8
{
	IdleToAim,
	Aiming,
	ShootToIdle,
	ShootToAim,
	AimToIdle
};

UCLASS()
class UNTILDAWN_API UGA_BowShoot : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_BowShoot();

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	void CreateAbilityTask();

	bool IsAbleShoot();

	void ShootArrow();

	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	void ProcessReleased();

	void ProcessPressed();

	void SetAbleShoot();

	void PlayMontage();

private:

	UPROPERTY(EditAnywhere, Category = "GAS | Animations")
	UAnimMontage* bowShootMontage;

	UPROPERTY(EditAnywhere, Category = "GAS | Animations")
	UAnimMontage* bowMeshMontage;

	UPROPERTY(VisibleAnywhere, Category = "GAS | Animations")
	EBowState bowState = EBowState::IdleToAim;

	UPROPERTY()
	USkeletalMeshComponent* bowMesh;

	UPROPERTY()
	TSubclassOf<UAnimInstance> bowAnimInstClass;

	TWeakObjectPtr<APlayerCharacter> character;

	TWeakObjectPtr<APlayerControllerMainMap> controller;

	UPROPERTY(EditAnywhere, Category = "GAS | Arrow")
	TSubclassOf<AActor> arrowClass;

	UPROPERTY()
	AActor* arrow;

	TWeakObjectPtr<UItemPermanent> weaponObject;

	bool bMontageEnded = false;

	FTimerHandle holdTimer;

	UPROPERTY(EditAnywhere, Category = "GAS | Option")
	float holdTime = 0.5f;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_HitReaction.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_HitReaction::UGA_HitReaction()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;
}

void UGA_HitReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const FHitResult& hitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TriggerEventData->TargetData, 0);

	UAbilityTask_PlayMontageAndWait* playHitReactionMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), hitReactionMontage, 1.f, GetHitReactDirection(hitResult.ImpactPoint));
	playHitReactionMontageTask->OnCompleted.AddDynamic(this, &UGA_HitReaction::OnCompleteCallback);
	playHitReactionMontageTask->OnInterrupted.AddDynamic(this, &UGA_HitReaction::OnInterruptedCallback);
	playHitReactionMontageTask->ReadyForActivation();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_HitReaction::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_HitReaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_HitReaction::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_HitReaction::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

FName UGA_HitReaction::GetHitReactDirection(const FVector& impactPoint)
{
	const FVector& actorLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
	float distanceToFrontBackPlane = FVector::PointPlaneDist(impactPoint, actorLocation, CurrentActorInfo->AvatarActor->GetActorRightVector());
	float distanceToRightLeftPlane = FVector::PointPlaneDist(impactPoint, actorLocation, CurrentActorInfo->AvatarActor->GetActorForwardVector());

	if (FMath::Abs(distanceToFrontBackPlane) <= FMath::Abs(distanceToRightLeftPlane))
	{
		return (distanceToRightLeftPlane >= 0 ? TEXT("F") : TEXT("B"));
	}
	else
	{
		return (distanceToFrontBackPlane >= 0 ? TEXT("R") : TEXT("L"));
	}
}

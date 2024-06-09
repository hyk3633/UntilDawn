// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_MeleeAttack.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"

UGA_MeleeAttack::UGA_MeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;
}

void UGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	controller = Cast<APlayerControllerMainMap>(character->GetController());
	SendAbilityActivationToController();
	if (meleeAttackMontage == nullptr)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	UAbilityTask_PlayMontageAndWait* playMeleeAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMeleeAttack"), meleeAttackMontage, 1.f, NAME_None);
	playMeleeAttackTask->OnCompleted.AddDynamic(this, &UGA_MeleeAttack::OnCompleteCallback);
	playMeleeAttackTask->OnInterrupted.AddDynamic(this, &UGA_MeleeAttack::OnInterruptedCallback);
	playMeleeAttackTask->ReadyForActivation();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_MeleeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UGA_MeleeAttack::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_MeleeAttack::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_MeleeAttack::SendAbilityActivationToController()
{
	if (controller.IsValid())
	{
		FGameplayAbilitySpec* specPtr = character->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(CurrentSpecHandle);
		controller->SendActivatedWeaponAbility(specPtr->InputID);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_Wrestling.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_Wrestling::UGA_Wrestling()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;
}

void UGA_Wrestling::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TWeakObjectPtr<APlayerCharacter> character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	TWeakObjectPtr<APlayerControllerMainMap> controller = Cast<APlayerControllerMainMap>(character->GetController());

	if (wrestlingMontage)
	{
		UAbilityTask_PlayMontageAndWait* playWresltingMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayWrestling"), wrestlingMontage, 1.f);
		playWresltingMontageTask->OnCompleted.AddDynamic(this, &UGA_Wrestling::OnCompleteCallback);
		playWresltingMontageTask->OnInterrupted.AddDynamic(this, &UGA_Wrestling::OnInterruptedCallback);
		playWresltingMontageTask->ReadyForActivation();

		if (controller.IsValid())
		{
			controller->WrestlingStart();
		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Wrestling::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Wrestling::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Wrestling::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Wrestling::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_JustPlayMontage.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_JustPlayMontage::UGA_JustPlayMontage()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;
}

void UGA_JustPlayMontage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAnimMontage* montage = nullptr;
	if (isPlayer && bNeedWeapon)
	{
		TWeakObjectPtr<APlayerCharacter> character = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
		montage = *montages.Find(character->GetCurrentWeaponType());
	}
	else
	{
		montage = montages[EWeaponType::NONE];
	}
	if (montage)
	{
		UAbilityTask_PlayMontageAndWait* playMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), montage, 1.f);
		playMontageTask->OnCompleted.AddDynamic(this, &UGA_JustPlayMontage::OnCompleteCallback);
		playMontageTask->OnInterrupted.AddDynamic(this, &UGA_JustPlayMontage::OnInterruptedCallback);
		playMontageTask->ReadyForActivation();

		if (isPlayer)
		{
			TWeakObjectPtr<APlayerCharacter> character = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
			TWeakObjectPtr<APlayerControllerMainMap> controller = Cast<APlayerControllerMainMap>(character->GetController());
			if (bShouldReplicate)
			{
				if (controller.IsValid())
				{
					FGameplayAbilitySpec* specPtr = character->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(CurrentSpecHandle);
					controller->SendActivatedWeaponAbility(specPtr->InputID);
				}
			}
			if (bCanMoving == false)
			{
				character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			}
		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_JustPlayMontage::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UGA_JustPlayMontage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (bCanMoving == false)
	{
		TWeakObjectPtr<APlayerCharacter> character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
		character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void UGA_JustPlayMontage::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_JustPlayMontage::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

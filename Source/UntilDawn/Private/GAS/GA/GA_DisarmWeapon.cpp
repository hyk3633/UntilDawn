// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_DisarmWeapon.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_DisarmWeapon::UGA_DisarmWeapon()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;
}

void UGA_DisarmWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TWeakObjectPtr<APlayerCharacter> character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	const EWeaponType weaponType = character->GetCurrentWeaponType();

	TWeakObjectPtr<APlayerControllerMainMap> controller = Cast<APlayerControllerMainMap>(character->GetController());
	if (controller.IsValid())
	{
		controller->DisarmWeapon();
	}

	if (montages.Find(weaponType))
	{
		UAbilityTask_PlayMontageAndWait* playDisarmWeaponMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), montages[weaponType], 1.f);
		playDisarmWeaponMontageTask->OnCompleted.AddDynamic(this, &UGA_DisarmWeapon::OnCompleteCallback);
		playDisarmWeaponMontageTask->OnInterrupted.AddDynamic(this, &UGA_DisarmWeapon::OnInterruptedCallback);
		playDisarmWeaponMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_DisarmWeapon::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UGA_DisarmWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UGA_DisarmWeapon::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_DisarmWeapon::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
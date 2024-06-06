// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_ArmWeapon.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "UntilDawn.h"

UGA_ArmWeapon::UGA_ArmWeapon()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;
}

void UGA_ArmWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TWeakObjectPtr<APlayerCharacter> character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	TWeakObjectPtr<APlayerControllerMainMap> controller = Cast<APlayerControllerMainMap>(character->GetController());
	if (controller.IsValid())
	{
		controller->ArmWeapon();
	}

	const EWeaponType weaponType = character->GetCurrentWeaponType();
	if (montages.Find(weaponType))
	{
		UAbilityTask_PlayMontageAndWait* playArmWeaponMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), montages[weaponType], 1.f);
		playArmWeaponMontageTask->OnCompleted.AddDynamic(this, &UGA_ArmWeapon::OnCompleteCallback);
		playArmWeaponMontageTask->OnInterrupted.AddDynamic(this, &UGA_ArmWeapon::OnInterruptedCallback);
		playArmWeaponMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_ArmWeapon::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UGA_ArmWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UGA_ArmWeapon::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void UGA_ArmWeapon::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

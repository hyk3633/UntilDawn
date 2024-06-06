// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_HitCheck.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Zombie/ZombieCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UntilDawn/UntilDawn.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_HitCheck::UGA_HitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;
}

void UGA_HitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TWeakObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	if (player.IsValid() == false)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	TWeakObjectPtr<APlayerControllerMainMap> playerController = Cast<APlayerControllerMainMap>(player->GetController());
	if (playerController.IsValid() == false)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	FVector collisionLocation = player->GetMesh()->GetSocketLocation(socketName);
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(player.Get());
	TArray<FHitResult> hits;
	UKismetSystemLibrary::SphereTraceMulti
	(
		player.Get(),
		collisionLocation,
		collisionLocation + FVector(0, 0, 1),
		sphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_PlayerAttack),
		false,
		actorsToIgnore,
		EDrawDebugTrace::Persistent,
		hits,
		true
	);

	playerController->SendHitResult(hits, triggerGameplayTag);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_HitCheck::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_HitCheck::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_HitCheck::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_HitCheck::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

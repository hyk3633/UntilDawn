// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_Sprint.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "GAS/AttributeSet/PlayerAttributeSet.h"
#include "Tag/UntilDawnGameplayTags.h"

UGA_Sprint::UGA_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;
}

bool UGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	TWeakObjectPtr<APlayerCharacter> avatarActor = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	const FGameplayAbilitySpec* abilitySpec = avatarActor->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(Handle);
	if (StaticCast<EInputType>(abilitySpec->InputID) != EInputType::Shift_Released)
	{
		return avatarActor->GetVelocity().Length() > 0.f;
	}
	return false;
}

void UGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	CancelSprint();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	
	character->GetCharacterMovement()->MaxWalkSpeed = 600;
	GetWorld()->GetTimerManager().SetTimer(gameplayEffectTimer, this, &UGA_Sprint::ConsumeStamina, timerRate, true);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Sprint::ConsumeStamina()
{
	const float currentStamina = character->GetAbilitySystemComponent()->GetNumericAttributeBase(UPlayerAttributeSet::GetStaminaAttribute());
	const float consumedStamina = currentStamina - staminaRate * timerRate;
	if (consumedStamina < 0.f)
	{
		CancelSprint();
	}
	else
	{
		character->GetAbilitySystemComponent()->SetNumericAttributeBase(UPlayerAttributeSet::GetStaminaAttribute(), consumedStamina);
	}
}

void UGA_Sprint::CancelSprint()
{
	character->GetCharacterMovement()->MaxWalkSpeed = 300;
	GetWorld()->GetTimerManager().ClearTimer(gameplayEffectTimer);
}

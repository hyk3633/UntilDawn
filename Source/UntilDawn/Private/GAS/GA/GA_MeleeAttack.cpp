// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_MeleeAttack.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "GAS/AttributeSet/PlayerAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"

UGA_MeleeAttack::UGA_MeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;
}

void UGA_MeleeAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (comboTimer.IsValid() == false)
	{
		hasNextComboInput = false;
	}
	else
	{
		hasNextComboInput = true;
	}
}

void UGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ensure(comboActivationSeconds.Num() > 0);

	character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	controller = Cast<APlayerControllerMainMap>(character->GetController());

	SendAbilityActivationToController();

	if (meleeAttackMontage == nullptr)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	playMeleeAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMeleeAttack"), meleeAttackMontage, 1.f, GetNextSection());
	playMeleeAttackTask->OnCompleted.AddDynamic(this, &UGA_MeleeAttack::OnCompleteCallback);
	playMeleeAttackTask->OnInterrupted.AddDynamic(this, &UGA_MeleeAttack::OnInterruptedCallback);
	playMeleeAttackTask->ReadyForActivation();

	StartComboTimer();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_MeleeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	playMeleeAttackTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	currentCombo = 0;
	hasNextComboInput = false;
}

FName UGA_MeleeAttack::GetNextSection()
{
	currentCombo = FMath::Clamp(currentCombo + 1, 1, comboActivationSeconds.Num());
	FName nextSection = *FString::FromInt(currentCombo);
	return nextSection;
}

void UGA_MeleeAttack::StartComboTimer()
{
	int32 comboIndex = currentCombo - 1;
	ensure(comboActivationSeconds.IsValidIndex(comboIndex));

	if (comboActivationSeconds[comboIndex] > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(comboTimer, this, &UGA_MeleeAttack::CheckComboInput, comboActivationSeconds[comboIndex], false);
	}
}

void UGA_MeleeAttack::CheckComboInput()
{
	comboTimer.Invalidate();
	if (hasNextComboInput)
	{
		if (CheckStamina())
		{
			SendAbilityActivationToController();
			MontageJumpToSection(GetNextSection());
			StartComboTimer();
			hasNextComboInput = false;
		}
		else
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}
}

bool UGA_MeleeAttack::CheckStamina()
{
	if (controller.IsValid())
	{
		const float currentStamina = character->GetAbilitySystemComponent()->GetNumericAttributeBase(UPlayerAttributeSet::GetStaminaAttribute());
		if (currentStamina > GetCostGameplayEffect()->Modifiers[0].Magnitude.GetValue())
		{
			FGameplayEffectSpecHandle effectSpecHandle = MakeOutgoingGameplayEffectSpec(GetCostGameplayEffect()->GetClass());
			if (effectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, effectSpecHandle);
			}
			return true;
		}
		return false;
	}
	else
	{
		return true;
	}
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

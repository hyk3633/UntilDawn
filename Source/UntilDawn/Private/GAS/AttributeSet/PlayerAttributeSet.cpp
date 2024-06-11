// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSet/PlayerAttributeSet.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "GameplayEffectExtension.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{

}

void UPlayerAttributeSet::InitStamina(const int newStamina)
{
	owningController = Cast<APlayerControllerMainMap>(GetOwningActor());
	SetStamina(newStamina);
	SetMaxStamina(newStamina);
}

void UPlayerAttributeSet::RecoverStamina(const float amount)
{
	SetStamina(FMath::Clamp(GetStamina() + amount, 0.f, GetMaxStamina()));
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = NewValue < 0.f ? 0.f : NewValue;
	}
}

void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	owningController->StaminaChanged(NewValue);
}

bool UPlayerAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (Super::PreGameplayEffectExecute(Data) == false)
		return false;

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		if (FMath::Abs(Data.EvaluatedData.Magnitude) > GetStamina())
		{
			Data.EvaluatedData.Magnitude = 0.f;
			return false;
		}
	}
	return true;
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

}

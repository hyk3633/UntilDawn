// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class APlayerControllerMainMap;

UCLASS()
class UNTILDAWN_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPlayerAttributeSet();

	void InitStamina(const int newStamina);

	void RecoverStamina(const float amount);

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxStamina);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	void SetInfinite();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	TWeakObjectPtr<APlayerControllerMainMap> owningController;

	bool bInfinite = false;
	
};
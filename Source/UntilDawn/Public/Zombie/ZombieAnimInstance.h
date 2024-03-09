// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */

class UAnimMontage;

UCLASS()
class UNTILDAWN_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	void PlayAttackMontage(const int8 attackAnimationType);

private:

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* attackMontage;
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/ZombieAnimInstance.h"

void UZombieAnimInstance::PlayAttackMontage(const int8 attackAnimationType)
{
	if (attackMontage)
	{
		Montage_Play(attackMontage);
		Montage_JumpToSection(FName(*FString::FromInt(attackAnimationType)));
	}
}

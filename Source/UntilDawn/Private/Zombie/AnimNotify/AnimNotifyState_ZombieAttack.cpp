// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/AnimNotify/AnimNotifyState_ZombieAttack.h"
#include "Zombie/ZombieCharacter.h"

UAnimNotifyState_ZombieAttack::UAnimNotifyState_ZombieAttack()
{
	
}

void UAnimNotifyState_ZombieAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	AZombieCharacter* zombie = Cast<AZombieCharacter>(MeshComp->GetOwner());
	if (IsValid(zombie))
	{
		zombie->StartAttack();
	}
}

void UAnimNotifyState_ZombieAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	elapsedTime += FrameDeltaTime;
	if (elapsedTime >= tickInterval)
	{
		AZombieCharacter* zombie = Cast<AZombieCharacter>(MeshComp->GetOwner());
		if (IsValid(zombie) && zombie->GetAttackActivated())
		{
			zombie->ActivateAttackTrace(attackAnimationType);
		}
		elapsedTime = 0.f;
	}
}

void UAnimNotifyState_ZombieAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	AZombieCharacter* zombie = Cast<AZombieCharacter>(MeshComp->GetOwner());
	if (IsValid(zombie))
	{
		if (zombie->GetAttackActivated())
		{
			zombie->SetAttackToPlayerResult(false);
		}
		zombie->EndAttack();
	}
	elapsedTime = 0.f;
}

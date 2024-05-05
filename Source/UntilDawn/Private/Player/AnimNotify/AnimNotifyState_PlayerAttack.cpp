// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AnimNotify/AnimNotifyState_PlayerAttack.h"
#include "Player/PlayerCharacter.h"

UAnimNotifyState_PlayerAttack::UAnimNotifyState_PlayerAttack()
{
}

void UAnimNotifyState_PlayerAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(player))
	{
		player->StartAttack();
	}
}

void UAnimNotifyState_PlayerAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	elapsedTime += FrameDeltaTime;
	if (elapsedTime >= tickInterval)
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (IsValid(player) && player->GetAttackActivated())
		{
			player->StartAttack();
		}
		elapsedTime = 0.f;
	}
}

void UAnimNotifyState_PlayerAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(player))
	{
		player->EndAttack();
	}
	elapsedTime = 0.f;
}

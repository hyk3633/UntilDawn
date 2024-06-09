// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AnimNotify/AnimNotifyState_PlayerAttack.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"

UAnimNotifyState_PlayerAttack::UAnimNotifyState_PlayerAttack()
{
}

void UAnimNotifyState_PlayerAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	TWeakObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player.IsValid())
	{
		playerController = Cast<APlayerControllerMainMap>(player->GetController());
		if (playerController.IsValid())
		{
			playerController->StartAttack();
			playerController->Attack();
		}
	}
}

void UAnimNotifyState_PlayerAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	elapsedTime += FrameDeltaTime;
	if (elapsedTime >= tickInterval)
	{
		if (playerController.IsValid() && playerController->IsAttacking())
		{
			playerController->Attack();
		}
		elapsedTime = 0.f;
	}
}

void UAnimNotifyState_PlayerAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (playerController.IsValid())
	{
		playerController->EndAttack();
	}
	playerController.Reset();
	elapsedTime = 0.f;
}

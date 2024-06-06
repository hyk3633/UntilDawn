// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotify/AnimNotifyState_InputInterval.h"
#include "Player/PlayerCharacter.h"

UAnimNotifyState_InputInterval::UAnimNotifyState_InputInterval()
{

}

void UAnimNotifyState_InputInterval::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	TWeakObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player.IsValid())
	{
		player->ActivateInputInterval();
	}
}

void UAnimNotifyState_InputInterval::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	TWeakObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player.IsValid())
	{
		player->DeactivateInputInterval();
	}
}

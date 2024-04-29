// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotify/AnimNotify_PlayerAttack.h"
#include "Player/PlayerCharacter.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(player))
	{
		player->StartAttack();
	}
}

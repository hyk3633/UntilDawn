// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotify/AnimNotify_PlayerAttack.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	auto player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	auto playerController = Cast<APlayerControllerMainMap>(player->GetController());
	if (IsValid(playerController))
	{
		playerController->StartAttack();
	}
}

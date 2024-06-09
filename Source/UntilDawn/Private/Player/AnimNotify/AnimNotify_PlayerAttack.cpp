// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotify/AnimNotify_PlayerAttack.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	TWeakObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player.IsValid())
	{
		TWeakObjectPtr<APlayerControllerMainMap> playerController = Cast<APlayerControllerMainMap>(player->GetController());
		if (playerController.IsValid())
		{
			playerController->Attack();
		}
	}
}

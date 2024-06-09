// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotify/AnimNotify_EventTrigger.h"
#include "Player/PlayerCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/UntilDawnGameplayTags.h"

UAnimNotify_EventTrigger::UAnimNotify_EventTrigger()
{
}

void UAnimNotify_EventTrigger::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	TWeakObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player.IsValid() && player->GetController())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(player.Get(), UD_EVENT_CHARACTER_HITCHECK, FGameplayEventData());
	}
}

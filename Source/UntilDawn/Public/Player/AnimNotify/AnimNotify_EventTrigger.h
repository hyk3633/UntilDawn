// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_EventTrigger.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UAnimNotify_EventTrigger : public UAnimNotify
{
	GENERATED_BODY()

public:

	UAnimNotify_EventTrigger();

private:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
private:

	

};

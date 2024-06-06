// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_InputInterval.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UAnimNotifyState_InputInterval : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	UAnimNotifyState_InputInterval();

private:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};

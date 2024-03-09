// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ZombieAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UAnimNotifyState_ZombieAttack : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UAnimNotifyState_ZombieAttack();

private:
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = "Option", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int attackAnimationType;

	UPROPERTY(EditAnywhere, Category = "Option", meta = (AllowPrivateAccess = "true", ClampMin = 0.016))
	float tickInterval = 0.03f;

	float elapsedTime = 0.f;

};

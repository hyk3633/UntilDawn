// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInst.generated.h"

/**
 * 
 */

class APlayerCharacter;

UCLASS()
class UNTILDAWN_API UPlayerAnimInst : public UAnimInstance
{
	GENERATED_BODY()

public:

	FORCEINLINE void SetMyCharacter(APlayerCharacter* character) { myCharacter = character; }

	UFUNCTION(BlueprintCallable)
	void UpdateValue();

private:

	UPROPERTY()
	APlayerCharacter* myCharacter;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float speed;

};

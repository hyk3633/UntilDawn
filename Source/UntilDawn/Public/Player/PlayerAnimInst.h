// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/WeaponType.h"
#include "PlayerAnimInst.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(DelegateMontageEnded);

class APlayerCharacter;
class UAnimMontage;

UCLASS()
class UNTILDAWN_API UPlayerAnimInst : public UAnimInstance
{
	GENERATED_BODY()

public:

	void InitAnimInst(APlayerCharacter* character);

	UFUNCTION(BlueprintCallable)
	void UpdateValue();

private:

	UPROPERTY()
	APlayerCharacter* myCharacter;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float speed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float velocityZ;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isFalling;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float direction;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float pitch;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float yaw;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool turnRight;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool turnLeft;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EWeaponType currentWeaponType = EWeaponType::NONE;
};

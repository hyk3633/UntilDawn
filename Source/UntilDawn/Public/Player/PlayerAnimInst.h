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
	void UpdateValue(float deltaTime);

protected:

	void FootIK(float deltaTime);

	TTuple<bool, float> CapsuleDistance(FName socketName);

	TTuple<bool, float, FVector> FootLineTrace(FName socketName);

private:

	UPROPERTY()
	APlayerCharacter* myCharacter;

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
	float targetSpeed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EWeaponType currentWeaponType = EWeaponType::NONE;

	UPROPERTY()
	TArray<AActor*> ignoreActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	float displacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	float IKInterpSpeed = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	FRotator RRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	FRotator LRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	float RIK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	float LIK;

};

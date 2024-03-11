// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/WeaponType.h"
#include "PlayerAnimInst.generated.h"

/**
 * 
 */

class APlayerCharacter;
class UAnimMontage;

UCLASS()
class UNTILDAWN_API UPlayerAnimInst : public UAnimInstance
{
	GENERATED_BODY()

public:

	FORCEINLINE void SetMyCharacter(APlayerCharacter* character) { myCharacter = character; }

	UFUNCTION(BlueprintCallable)
	void UpdateValue();

	void PlayWeaponArmMontage(const EWeaponType type);

	void PlayWeaponDisarmMontage(const EWeaponType type);

	void PlayAxeAttackMontage();

	void PlayBowDrawMontage();

	void PlayBowShootMontage();

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
	bool turnRight;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool turnLeft;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool rightClick;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isAbleShoot;

	UPROPERTY(EditDefaultsOnly, Category = "Montages", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* axeArmDisarmMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* bowArmDisarmMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* bowDrawMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* axeAttackMontage;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EWeaponType currentWeaponType = EWeaponType::DEFAULT;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isWrestling;
};

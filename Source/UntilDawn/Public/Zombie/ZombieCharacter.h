// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/ZombieState.h"
#include "Structs/Pos.h"
#include "Structs/CharacterInfo.h"
#include "../Interface/PoolableActor.h"
#include "ZombieCharacter.generated.h"

class UZombieAnimInstance;
class APlayerCharacter;
class UWidgetComponent;
class UWidgetZombieHealth;

DECLARE_DELEGATE_OneParam(OnHealthChanged, float healthPercentage);

UCLASS()
class UNTILDAWN_API AZombieCharacter : public ACharacter, public IPoolableActor
{
	GENERATED_BODY()

public:

	AZombieCharacter();

	virtual void ActivateActor() override;

	virtual void DeactivateActor() override;

	virtual bool IsActorActivated() override;

	void SetZombieInfo(const ZombieInfo& info);

	void ZombieDead();

	OnHealthChanged onHealthChanged;

protected:

	void ProcessZombieInfo(const ZombieInfo& info, const int bitType);

	void DeactivateAfterDelay();

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetNumber(const int newNumber) { number = newNumber; }

	FORCEINLINE const int GetNumber() const { return number; }

	FORCEINLINE void SetTarget(APlayerCharacter* target) { targetPlayer = target; }

	void SetZombieState(const EZombieState newState);

	UFUNCTION(BlueprintCallable)
	EZombieState GetZombieState() const;

	UFUNCTION(BlueprintCallable)
	float GetSpeed() const;

	void SetNextLocation(const FVector& nextLoc);

protected:

	void UpdateMovement();

	void FollowPath();

	void StartMovementUpdate();

public:

	void StartAttack();

	void EndAttack();

	FORCEINLINE int GetAttackActivated() const { return isAttackActivated; }

	void ActivateAttackTrace(const int attackAnimationType);

	void SetAttackToPlayerResult(const bool result);

	void UpdateHealth(const float newHealth);

protected:

	void HideHealthWidget();

private:

	UPROPERTY()
	UZombieAnimInstance* animInst;

	UPROPERTY()
	UWidgetComponent* healthWidget;

	TWeakObjectPtr<UWidgetZombieHealth> healthWidgetObject;

	float health = 200;

	float maxHealth = 200;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	bool isActive;

	int number;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	EZombieState state;

	float speed = 100.f;

	float interval = 0.016f;

	FVector nextPoint, nextDirection;

	UPROPERTY()
	APlayerCharacter* targetPlayer;

	FTimerHandle movementUpdateTimer;

	bool isAttackActivated;

	int16 damage = 30;

	FTimerHandle deactivateDelayTimer;

	FTimerHandle healthWidgetHideTimer;

};

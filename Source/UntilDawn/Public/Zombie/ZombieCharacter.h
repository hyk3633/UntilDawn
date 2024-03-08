// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/ZombieState.h"
#include <vector>
#include "Structs/Pos.h"
#include "ZombieCharacter.generated.h"
using std::vector;

UCLASS()
class UNTILDAWN_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AZombieCharacter();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ActivateActor();

	void DeactivateActor();

	FORCEINLINE const bool GetIsActive() const { return isActive; }

	FORCEINLINE void SetNumber(const int newNumber) { number = newNumber; }

	FORCEINLINE const int GetNumber() const { return number; }

	FORCEINLINE void SetTarget(ACharacter* target) { targetPlayer = target; }

	void SetZombieState(const EZombieState newState);

	UFUNCTION(BlueprintCallable)
	EZombieState GetZombieState() const;

	UFUNCTION(BlueprintCallable)
	float GetSpeed() const;

	void SetNextLocation(const FVector& nextLoc);

protected:

	void InitializePathStatus();

	void UpdateMovement();

	void FollowPath();

	void StartMovementUpdate();

private:

	bool isActive;

	int number;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	EZombieState state;

	float speed = 100.f;

	float interval = 0.016f;

	FVector nextPoint, nextDirection;

	ACharacter* targetPlayer;

	FTimerHandle movementUpdateTimer;

};

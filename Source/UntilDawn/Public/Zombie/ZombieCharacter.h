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

	void SetZombieState(const EZombieState newState);

	UFUNCTION(BlueprintCallable)
	EZombieState GetZombieState() const;

	FORCEINLINE void SetVelocity(const FVector& vel) { velocity = vel; }

	void SetPath(const vector<Pos>& path);

	void InitializePathStatus();

	UFUNCTION(BlueprintCallable)
	float GetSpeed() const;

	void UpdateMovement();

	void FollowPath();

	FORCEINLINE void SetTarget(ACharacter* target) { targetPlayer = target; }

	void StartMovementUpdate();

private:

	bool isActive;

	int number;

	EZombieState state;

	FVector velocity;

	vector<Pos> pathToTarget;

	float speed;

	float elapsed;

	int pathIdx;

	FVector nextPoint, nextDirection;

	ACharacter* targetPlayer;

	FTimerHandle movementUpdateTimer;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/ZombieState.h"
#include "ZombieCharacter.generated.h"

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

	FORCEINLINE void SetZombieState(const EZombieState newState) { state = newState; }

	UFUNCTION(BlueprintCallable)
	EZombieState GetZombieState() const;

	UFUNCTION(BlueprintCallable)
	float GetSpeed() const;

	FORCEINLINE void SetTargetLocation(FVector loc) { TargetLoc = loc; }

	void Move();

private:

	bool isActive;

	int number;

	EZombieState state;

	FVector TargetLoc;

	bool bMoving;

};

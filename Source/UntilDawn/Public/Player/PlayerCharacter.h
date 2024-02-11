// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;

UCLASS()
class UNTILDAWN_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* newController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

public:	

	virtual void Tick(float deltaTime) override;

	FORCEINLINE void SetPlayerId(const FString& id) { PlayerId = id; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* followCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* defaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* jumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* lookAction;

	FString PlayerId;

};

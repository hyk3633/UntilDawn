// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class UNTILDAWN_API AProjectileBase : public AItemBase
{
	GENERATED_BODY()
	
public:	

	AProjectileBase();

	virtual void ActivateActor() override;

	virtual void DeactivateActor() override;

	virtual bool IsActorActivated() override;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);

public:	

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	UParticleSystemComponent* particleComponent;

	UPROPERTY()
	UProjectileMovementComponent* movementComponent;

	UPROPERTY()
	USphereComponent* collision;
	
	bool isActive;
};

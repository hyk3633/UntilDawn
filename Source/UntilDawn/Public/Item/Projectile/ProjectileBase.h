// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Interface/PoolableActor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class UNTILDAWN_API AProjectileBase : public AActor, public IPoolableActor
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
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DeactivateAfterDelay();

public:	

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* particleComponent;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* movementComponent;

	UPROPERTY(EditAnywhere)
	USphereComponent* collision;
	
	bool isActive;

	FTimerHandle deactivateTimer;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Interface/PoolableActor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class UNTILDAWN_API AProjectileBase : public AActor, public IPoolableActor
{
	GENERATED_BODY()
	
public:	

	AProjectileBase();

protected:

	virtual void BeginPlay() override;

public:

	/* 풀러블 액터 인터페이스 가상 함수 */

	virtual void ActivateActor() override;

	virtual void DeactivateActor() override;

	virtual bool IsActorActivated() override;

	void ActivateProjectile();

protected:

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DeactivateAfterDelay();

public:

	void SetAttackPower(const float atkPower);

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const { return skeletalMesh; }

private:

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* particleComponent;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* movementComponent;

	UPROPERTY(EditAnywhere)
	USphereComponent* collision;

	UPROPERTY(EditAnywhere)
	UParticleSystem* brickImpactParticle;

	UPROPERTY(EditAnywhere)
	USoundCue* brickImpactSound;
	
	bool isActive;

	FTimerHandle deactivateTimer;

	float attackPower;
};

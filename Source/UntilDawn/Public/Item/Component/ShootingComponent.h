// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Interface/AttackShooting.h"
#include "ShootingComponent.generated.h"

class AProjectileBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UShootingComponent : public UActorComponent, public IAttackShooting
{
	GENERATED_BODY()

public:	

	UShootingComponent();

protected:

	virtual void BeginPlay() override;

public:

	FVector GetAimLocaion(TWeakObjectPtr<APlayerController> shooterController);

	virtual void Shooting(TWeakObjectPtr<APlayerController> shooterController, USkeletalMeshComponent* weaponMesh, TWeakObjectPtr<AProjectileBase> projectile) override;

};

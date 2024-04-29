// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackShooting.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackShooting : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNTILDAWN_API IAttackShooting
{
	GENERATED_BODY()

public:

	virtual void Shooting(TWeakObjectPtr<APlayerController> shooterController, USkeletalMeshComponent* weaponMesh, TWeakObjectPtr<class AProjectileBase> projectile) = 0;

};

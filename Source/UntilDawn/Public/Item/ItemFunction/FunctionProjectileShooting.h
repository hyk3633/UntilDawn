// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class APlayerController;
class USkeletalMeshComponent;

class UNTILDAWN_API FunctionProjectileShooting
{
public:

	FunctionProjectileShooting();
	~FunctionProjectileShooting();

protected:

	FVector GetAimLocaion(TWeakObjectPtr<APlayerController> attackerController);

public:

	void Shooting(TWeakObjectPtr<APlayerController> attackerController, USkeletalMeshComponent* weaponMesh);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class APlayerControllerMainMap;
class USkeletalMeshComponent;

class UNTILDAWN_API FunctionProjectileShooting
{
public:

	FunctionProjectileShooting();
	~FunctionProjectileShooting();

protected:

	FVector GetAimLocaion(TWeakObjectPtr<APlayerControllerMainMap> attackerController);

public:

	void Shooting(TWeakObjectPtr<APlayerControllerMainMap> attackerController, USkeletalMeshComponent* weaponMesh, const float atkPower);

};

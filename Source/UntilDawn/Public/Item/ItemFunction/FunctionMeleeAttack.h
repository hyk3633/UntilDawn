// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class APlayerControllerMainMap;

class UNTILDAWN_API FunctionMeleeAttack
{
public:
	FunctionMeleeAttack();
	~FunctionMeleeAttack();

protected:

public:

	void MeleeAttack(TWeakObjectPtr<APlayerControllerMainMap> attackerController, USkeletalMeshComponent* weaponMesh, const FString& itemID);

};

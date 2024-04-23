// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UMeleeAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UMeleeAttackComponent();

protected:

	virtual void BeginPlay() override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interface/PoolableActor.h"
#include "ProjectileBase.generated.h"

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

public:	

	virtual void Tick(float DeltaTime) override;

	

};

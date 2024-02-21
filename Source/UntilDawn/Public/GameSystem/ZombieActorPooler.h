// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZombieActorPooler.generated.h"

class AZombieCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UZombieActorPooler : public UActorComponent
{
	GENERATED_BODY()

public:

	UZombieActorPooler();

protected:

	virtual void BeginPlay() override;

public:

	FORCEINLINE TArray<AZombieCharacter*>& GetActorPool() { return zombiePool; }

	FORCEINLINE void SetPoolSize(const int size) { poolSize = size; }

	FORCEINLINE const int8 GetPoolSize() { return poolSize; }

	AZombieCharacter* GetPooledActor();

public:	

	UPROPERTY()
	TArray<AZombieCharacter*> zombiePool;

	int8 poolSize;
};

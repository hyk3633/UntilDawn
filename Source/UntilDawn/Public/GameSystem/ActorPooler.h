// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Enums/PoolableActorType.h"
#include "../Interface/PoolableActor.h"
#include "ActorPooler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UActorPooler : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActorPooler();

	void SetActorClass(UClass* newClass);

	void SpawnPoolableActor(const int spawnCount);

	FORCEINLINE void SetPoolSize(const int size) { poolSize = size; } // 액터 추가되면 값 변하게

	FORCEINLINE const int8 GetPoolSize() const { return poolSize; }

	TWeakObjectPtr<AActor> GetPooledActor();

private:

	UPROPERTY()
	UClass* actorClass;

	UPROPERTY()
	TArray<AActor*> actorPool;

	int8 poolSize;

};

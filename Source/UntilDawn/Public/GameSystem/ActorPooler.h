// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Enums/PoolableActorType.h"
#include "../Interface/PoolableActor.h"
#include "ActorPooler.generated.h"

USTRUCT()
struct FActorPool
{
	GENERATED_BODY()

public:

	FActorPool() {}

	UPROPERTY()
	TArray<AActor*> actorPool;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UActorPooler : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActorPooler();

	bool IsActorTypeExist(const int itemType);

	void SpawnPoolableActor(const int itemType, UClass* actorClass, const int spawnCount);

	FORCEINLINE void SetPoolSize(const int size) { poolSize = size; } // 액터 추가되면 값 변하게

	FORCEINLINE const int8 GetPoolSize() const { return poolSize; }

	TWeakObjectPtr<AActor> GetPooledActor(const int itemType);

private:

	UPROPERTY()
	TMap<int, FActorPool> actorPoolMap;

	int8 poolSize;

};

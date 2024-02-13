// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPooler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UActorPooler : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActorPooler();

protected:

	virtual void BeginPlay() override;

public:	

	FORCEINLINE TArray<AActor*>& GetActorPool() { return actorPool; }

	FORCEINLINE void SetPoolSize(const int size) { poolSize = size; }

	AActor* GetPooledActor();

private:

	UPROPERTY()
	TArray<AActor*> actorPool;

	int poolSize;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/ActorSpawner.h"

UActorSpawner::UActorSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UActorSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void UActorSpawner::SpawnActor(TSubclassOf<AActor> actorClass, int spawnCount, TArray<AActor*>& actorArray)
{
	for (int i = 0; i < spawnCount; i++)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(actorClass, FVector(0, 0, -3500), FRotator::ZeroRotator);
		actorArray.Add(actor);
	}
}


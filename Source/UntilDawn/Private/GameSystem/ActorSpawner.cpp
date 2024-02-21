// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSystem/ActorSpawner.h"
#include "Zombie/ZombieCharacter.h"

UActorSpawner::UActorSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void UActorSpawner::SpawnActor(const int spawnCount, TArray<AZombieCharacter*>& actorArray)
{
	for (int i = 0; i < spawnCount; i++)
	{
		AZombieCharacter* actor = GetWorld()->SpawnActor<AZombieCharacter>(AZombieCharacter::StaticClass(), FVector(0, 0, -3500), FRotator::ZeroRotator);
		actor->SpawnDefaultController();
		actorArray.Add(actor);
	}
}
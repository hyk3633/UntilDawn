// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interface/PoolableActor.h"
#include "../Enums/ItemType.h"
#include "ActorSpawner.generated.h"

class AZombieCharacter;
class AItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UActorSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActorSpawner();

public:	

	void SpawnZombie(const int spawnCount, TArray<AActor*>& actorArray);
		
	void SpawnItem(const int spawnCount, const EItemMainType type, TArray<AActor*>& actorArray);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorSpawner.generated.h"

class AZombieCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UActorSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActorSpawner();

protected:

	virtual void BeginPlay() override;

public:	
		
	void SpawnActor(const int spawnCount, TArray<AZombieCharacter*>& actorArray);

};

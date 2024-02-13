// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorSpawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UActorSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActorSpawner();

protected:

	virtual void BeginPlay() override;

public:	
		
	void SpawnActor(TSubclassOf<AActor> actorClass, int spawnCount, TArray<AActor*>& actorArray);

};

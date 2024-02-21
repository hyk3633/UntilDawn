// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSystem/ZombieActorPooler.h"
#include "Zombie/ZombieCharacter.h"

UZombieActorPooler::UZombieActorPooler()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UZombieActorPooler::BeginPlay()
{
	Super::BeginPlay();

	
}

AZombieCharacter* UZombieActorPooler::GetPooledActor()
{
	for (AZombieCharacter* zombie : zombiePool)
	{
		if (zombie->GetIsActive() == false)
		{
			zombie->ActivateActor();
			return zombie;
		}
	}
	return nullptr;
}


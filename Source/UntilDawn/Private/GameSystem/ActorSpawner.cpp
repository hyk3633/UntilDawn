// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSystem/ActorSpawner.h"
#include "Zombie/ZombieCharacter.h"
#include "Item/Weapon/ItemMeleeWeapon.h"

UActorSpawner::UActorSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorSpawner::SpawnActor(const int spawnCount, const EPoolableActorType type, TArray<AActor*>& actorArray)
{
	AActor* actor = nullptr;
	for (int i = 0; i < spawnCount; i++)
	{
		if (type == EPoolableActorType::Zombie)
		{
			actor = GetWorld()->SpawnActor<AZombieCharacter>(AZombieCharacter::StaticClass(), FVector(0, 0, -3500), FRotator::ZeroRotator);
		}
		else if (type == EPoolableActorType::MeleeWeapon)
		{
			actor = GetWorld()->SpawnActor<AItemMeleeWeapon>(AItemMeleeWeapon::StaticClass(), FVector(0, 0, -3500), FRotator::ZeroRotator);
		}
		actorArray.Add(actor);
	}
}
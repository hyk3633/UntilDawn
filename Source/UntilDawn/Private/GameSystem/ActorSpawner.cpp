// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSystem/ActorSpawner.h"
#include "Zombie/ZombieCharacter.h"
#include "Item/Weapon/ItemMeleeWeapon.h"

UActorSpawner::UActorSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorSpawner::SpawnZombie(const int spawnCount, TArray<AActor*>& actorArray)
{
	AActor* actor = nullptr;
	for (int i = 0; i < spawnCount; i++)
	{
		actor = GetWorld()->SpawnActor<AZombieCharacter>(AZombieCharacter::StaticClass(), FVector(0, 0, -3500), FRotator::ZeroRotator);
		actorArray.Add(actor);
	}
}

void UActorSpawner::SpawnItem(const int spawnCount, const EItemMainType type, TArray<AActor*>& actorArray)
{
	AActor* actor = nullptr;
	for (int i = 0; i < spawnCount; i++)
	{
		if (type == EItemMainType::MeleeWeapon)
		{
			actor = GetWorld()->SpawnActor<AItemMeleeWeapon>(AItemMeleeWeapon::StaticClass(), FVector(0, 0, -3500), FRotator::ZeroRotator);
		}
		else if (type == EItemMainType::RangedWeapon)
		{
			actor = GetWorld()->SpawnActor<AItemBase>(AItemBase::StaticClass(), FVector(0, 0, -3500), FRotator::ZeroRotator);
		}
		else if (type == EItemMainType::RecoveryItem)
		{

		}
		else if (type == EItemMainType::AmmoItem)
		{

		}
		actorArray.Add(actor);
	}
}
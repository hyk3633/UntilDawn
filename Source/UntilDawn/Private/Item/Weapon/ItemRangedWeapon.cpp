// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/ItemRangedWeapon.h"
#include "GameSystem/ActorPooler.h"
#include "Item/Projectile/ProjectileBase.h"

AItemRangedWeapon::AItemRangedWeapon()
{
	projectilePooler = CreateDefaultSubobject<UActorPooler>(TEXT("Projectile Pooler"));
}

void AItemRangedWeapon::BeginPlay()
{
	Super::BeginPlay();

	projectilePooler->SpawnPoolableActor(AProjectileBase::StaticClass(), 15);
}



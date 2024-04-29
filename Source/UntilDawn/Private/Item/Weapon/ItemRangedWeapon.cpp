// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/ItemRangedWeapon.h"
#include "Item/Projectile/ProjectileBase.h"
#include "Item/Component/ShootingComponent.h"
#include "Item/ItemObject.h"
#include "Structs/ItemInfo.h"
#include "GameSystem/ActorPooler.h"

AItemRangedWeapon::AItemRangedWeapon()
{
	shootingComponent = CreateDefaultSubobject<UShootingComponent>(TEXT("Shooting Component"));

	projectilePooler = CreateDefaultSubobject<UActorPooler>(TEXT("Projectile Pooler"));
}

void AItemRangedWeapon::InitializeRangedWeaponInfo(const FRangedWeaponInfo& newInfo)
{
	rangedWeaponInfo = newInfo;
}

void AItemRangedWeapon::Attack(TWeakObjectPtr<APlayerController> ownerController)
{
	TWeakObjectPtr<AProjectileBase> projectile = Cast<AProjectileBase>(projectilePooler->GetPooledActor(0));
	check(projectile.Get());
	shootingComponent->Shooting(ownerController, GetSkeletalMesh(), projectile);
}

EWeaponType AItemRangedWeapon::GetWeaponType() const
{
	return static_cast<EWeaponType>(rangedWeaponInfo.weaponType);
}

void AItemRangedWeapon::BeginPlay()
{
	Super::BeginPlay();

	projectilePooler->SpawnPoolableActor(0, AProjectileBase::StaticClass(), 15);
}



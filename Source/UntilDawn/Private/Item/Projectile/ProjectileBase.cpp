// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Projectile/ProjectileBase.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AProjectileBase::ActivateActor()
{
}

void AProjectileBase::DeactivateActor()
{
}

bool AProjectileBase::IsActorActivated()
{
	return false;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




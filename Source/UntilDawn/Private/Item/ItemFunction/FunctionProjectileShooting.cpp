// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFunction/FunctionProjectileShooting.h"
#include "Item/Projectile/ProjectileBase.h"
#include "GameMode/GameModeMainMap.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "../UntilDawn.h"

FunctionProjectileShooting::FunctionProjectileShooting()
{
}

FunctionProjectileShooting::~FunctionProjectileShooting()
{
}

FVector FunctionProjectileShooting::GetAimLocaion(TWeakObjectPtr<APlayerController> attackerController)
{
	check(GEngine);
	check(GEngine->GameViewport);

	FVector2D ViewPortSize;
	GEngine->GameViewport->GetViewportSize(ViewPortSize);
	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector startLoc, dir;
	UGameplayStatics::DeprojectScreenToWorld(attackerController.Get(), CrosshairLocation, startLoc, dir);
	FHitResult hit;
	attackerController->GetWorld()->LineTraceSingleByChannel
	(
		hit,
		startLoc,
		startLoc + dir * 5000.f,
		ECC_AimTrace
	);

	if (hit.bBlockingHit)
	{
		return hit.ImpactPoint;
	}
	else
	{
		return startLoc + dir * 5000.f;
	}
}

void FunctionProjectileShooting::Shooting(TWeakObjectPtr<APlayerController> attackerController, USkeletalMeshComponent* weaponMesh)
{
	check(attackerController.IsValid());

	FVector aimLocation = GetAimLocaion(attackerController);
	FVector muzzleLocation = weaponMesh->GetSocketLocation(FName("MuzzleSocket"));
	const FVector direction = (aimLocation - muzzleLocation).GetSafeNormal();

	auto projectile = attackerController->GetWorld()->GetAuthGameMode<AGameModeMainMap>()->GetProjectile();

	projectile->SetOwner(attackerController.Get());
	projectile->SetActorLocation(muzzleLocation + direction * -50.f);
	projectile->SetActorRotation(direction.Rotation());
	projectile->ActivateActor();
}

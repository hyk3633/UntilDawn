// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Component/ShootingComponent.h"
#include "Item/Projectile/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "../UntilDawn.h"

UShootingComponent::UShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector UShootingComponent::GetAimLocaion(TWeakObjectPtr<APlayerController> shooterController)
{
	check(GEngine);
	check(GEngine->GameViewport);

	FVector2D ViewPortSize;
	GEngine->GameViewport->GetViewportSize(ViewPortSize);
	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector startLoc, dir;
	UGameplayStatics::DeprojectScreenToWorld(shooterController.Get(), CrosshairLocation, startLoc, dir);
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel
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

void UShootingComponent::Shooting(TWeakObjectPtr<APlayerController> shooterController, USkeletalMeshComponent* weaponMesh, TWeakObjectPtr<AProjectileBase> projectile)
{
	check(shooterController.IsValid());

	FVector aimLocation = GetAimLocaion(shooterController);
	FVector muzzleLocation = weaponMesh->GetSocketLocation(FName("MuzzleSocket"));
	const FVector direction = (aimLocation - muzzleLocation).GetSafeNormal();

	projectile->SetActorLocation(muzzleLocation);
	projectile->SetActorRotation(direction.Rotation());
	projectile->ActivateActor();
}


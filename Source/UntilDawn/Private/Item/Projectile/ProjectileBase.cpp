// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Projectile/ProjectileBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	SetRootComponent(collision);
	collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetSkeletalMesh()->SetupAttachment(RootComponent);

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->ProjectileGravityScale = 0.f;
	movementComponent->SetAutoActivate(false);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset(TEXT("SkeletalMesh'/Game/G2_SurvivalCharacters/Meshes/Weapons/SK_Arrow.SK_Arrow'"));
	if (skeletalMeshAsset.Succeeded()) { GetSkeletalMesh()->SetSkeletalMesh(skeletalMeshAsset.Object); }
}

void AProjectileBase::ActivateActor()
{
	Super::ActivateActor();

	GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	collision->SetCollisionProfileName(FName("ProjectileCollision"));

	movementComponent->SetUpdatedComponent(RootComponent);
	movementComponent->SetVelocityInLocalSpace(FVector(50, 0, 0));
	movementComponent->Activate();
}

void AProjectileBase::DeactivateActor()
{
	Super::DeactivateActor();

	collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AProjectileBase::IsActorActivated()
{
	return isActive;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	movementComponent->OnProjectileStop.AddDynamic(this, &AProjectileBase::OnImpact);
}

void AProjectileBase::OnImpact(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		movementComponent->Deactivate();
		collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Projectile/ProjectileBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "../../UntilDawn/UntilDawn.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/UntilDawnGameplayTags.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	SetRootComponent(collision);
	collision->SetCollisionObjectType(ECC_Projectile);
	collision->SetCollisionProfileName(FName("ProjectileCollision"));
	collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	collision->SetSphereRadius(5.f);
	collision->SetSimulatePhysics(false);
	collision->SetNotifyRigidBodyCollision(true);

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	skeletalMesh->SetupAttachment(RootComponent);
	skeletalMesh->SetupAttachment(RootComponent);
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	skeletalMesh->SetRelativeLocation(FVector(-96.f, 0.f, 0.f));
	skeletalMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	skeletalMesh->SetWorldScale3D(FVector(2, 1, 2));

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->ProjectileGravityScale = 0.1f;
	movementComponent->SetAutoActivate(false);
	movementComponent->MaxSpeed = 3000.f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset(TEXT("SkeletalMesh'/Game/G2_SurvivalCharacters/Meshes/Weapons/SK_Arrow.SK_Arrow'"));
	if (skeletalMeshAsset.Succeeded()) { skeletalMesh->SetSkeletalMesh(skeletalMeshAsset.Object); }
}

void AProjectileBase::ActivateActor()
{
	skeletalMesh->SetVisibility(true);
	isActive = true;
}

void AProjectileBase::DeactivateActor()
{
	SetOwner(nullptr);

	skeletalMesh->SetVisibility(false);

	collision->SetSimulatePhysics(false);
	collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	movementComponent->Deactivate();

	isActive = false;

	attackPower = 0.f;
}

bool AProjectileBase::IsActorActivated()
{
	return isActive;
}

void AProjectileBase::ActivateProjectile()
{
	movementComponent->SetUpdatedComponent(RootComponent);
	movementComponent->SetVelocityInLocalSpace(FVector(-1000, 0, 0));
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->bInterpMovement = true;
	movementComponent->Activate(true);

	collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	collision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	check(attackPower >= 0.f);
	TWeakObjectPtr<APlayerControllerMainMap> owner = Cast<APlayerControllerMainMap>(GetOwner());
	if (owner.IsValid())
	{		
		GetWorldTimerManager().SetTimer(deactivateTimer, this, &AProjectileBase::DeactivateAfterDelay, 5.f);

		TWeakObjectPtr<ACharacter> character = Cast<ACharacter>(Hit.GetActor());
		if (character.IsValid() == false)
			return;

		TArray<FHitResult> hits;
		hits.Add(Hit);
		owner->SendHittedCharacters(hits, attackPower);

		FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
		FGameplayEventData payloadData;
		payloadData.TargetData.Add(targetData);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Hit.GetActor(), UD_EVENT_CHARACTER_HITREACTION, payloadData);
	}
}

void AProjectileBase::DeactivateAfterDelay()
{
	DeactivateActor();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::SetAttackPower(const float atkPower)
{
	attackPower = atkPower;
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Projectile/ProjectileBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject.h"
#include "Item/ItemObject/ItemPermanent.h"
#include "../../UntilDawn/UntilDawn.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/UntilDawnGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	SetRootComponent(collision);
	collision->SetCollisionObjectType(ECC_Projectile);
	collision->SetCollisionProfileName(FName("ProjectileCollision"));
	collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	collision->SetSphereRadius(2.f);
	collision->SetSimulatePhysics(false);
	collision->SetNotifyRigidBodyCollision(true);

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	skeletalMesh->SetupAttachment(RootComponent);
	skeletalMesh->SetupAttachment(RootComponent);
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	skeletalMesh->SetRelativeLocation(FVector(-96.f, 0.f, 0.f));
	skeletalMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	skeletalMesh->SetWorldScale3D(FVector(2.f, 0.75f, 2.f));

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->ProjectileGravityScale = 0.1f;
	movementComponent->SetAutoActivate(false);
	movementComponent->MaxSpeed = 3000.f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset(TEXT("SkeletalMesh'/Game/G2_SurvivalCharacters/Meshes/Weapons/SK_Arrow.SK_Arrow'"));
	if (skeletalMeshAsset.Succeeded()) { skeletalMesh->SetSkeletalMesh(skeletalMeshAsset.Object); }

	static ConstructorHelpers::FObjectFinder<UParticleSystem> brickImpactParticleRef(TEXT("ParticleSystem'/Game/MilitaryWeapDark/FX/P_Impact_Stone_Small_01.P_Impact_Stone_Small_01'"));
	if (brickImpactParticleRef.Succeeded()) { brickImpactParticle = brickImpactParticleRef.Object; }

	static ConstructorHelpers::FObjectFinder<USoundCue> brickImpactSoundRef(TEXT("SoundCue'/Game/_Assets/Sounds/Item/Bow/SC_Bow_Impact_Brick.SC_Bow_Impact_Brick'"));
	if (brickImpactSoundRef.Succeeded()) { brickImpactSound = brickImpactSoundRef.Object; }

}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	collision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);
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

	DetachRootComponentFromParent();
}

bool AProjectileBase::IsActorActivated()
{
	return isActive;
}

void AProjectileBase::ActivateProjectile()
{
	movementComponent->SetUpdatedComponent(RootComponent);
	movementComponent->SetVelocityInLocalSpace(FVector(3000, 0, 0));
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->bInterpMovement = true;
	movementComponent->Activate(true);

	collision->IgnoreActorWhenMoving(GetOwner(), true);
	collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetWorldTimerManager().SetTimer(deactivateTimer, this, &AProjectileBase::DeactivateAfterDelay, 7.f);
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);

	TWeakObjectPtr<ACharacter> character = Cast<ACharacter>(Hit.GetActor());
	if (character.IsValid() == false)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, brickImpactParticle, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), true);
		UGameplayStatics::PlaySoundAtLocation(this, brickImpactSound, Hit.ImpactPoint);
	}

	TWeakObjectPtr<APlayerControllerMainMap> owner = Cast<APlayerControllerMainMap>(GetOwner());
	if (owner.IsValid())
	{		
		TWeakObjectPtr<APlayerCharacter> shooter = Cast<APlayerCharacter>(owner->GetPawn());

		owner->SendHittedCharacter(Hit, shooter->GetArmedWeapon()->GetItemID());

		FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
		FGameplayEventData payloadData;
		payloadData.TargetData.Add(targetData);

		TWeakObjectPtr<UItemPermanent> weaponObj = Cast<UItemPermanent>(shooter->GetArmedWeapon()->GetItemObject());
		if (weaponObj.IsValid())
		{
			payloadData.EventMagnitude = weaponObj->GetItemSubType();
		}
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Hit.GetActor(), UD_EVENT_CHARACTER_HITREACTION, payloadData);
	}
}

void AProjectileBase::DeactivateAfterDelay()
{
	DeactivateActor();
}

void AProjectileBase::SetAttackPower(const float atkPower)
{
	attackPower = atkPower;
}




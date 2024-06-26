// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie/ZombieCharacter.h"
#include "Zombie/ZombieAIController.h"
#include "Zombie/ZombieAnimInstance.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "UI/Main/WidgetZombieHealth.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UntilDawn/UntilDawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Tag/UntilDawnGameplayTags.h"
#include "Engine/AssetManager.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"

AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AZombieAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::Spawned;

	asc = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	static ConstructorHelpers::FClassFinder<UGameplayAbility> hitReactionBP(TEXT("Blueprint'/Game/_Assets/Blueprints/GAS/GA/Zombie/BPGA_Zombie_HitReaction.BPGA_Zombie_HitReaction_C'"));
	if (hitReactionBP.Succeeded()) hitReactionAbility = hitReactionBP.Class;

	static ConstructorHelpers::FClassFinder<UGameplayAbility> kickReactionAbilityBP(TEXT("'/Game/_Assets/Blueprints/GAS/GA/Zombie/BPGA_Zombie_KickReaction.BPGA_Zombie_KickReaction_C'"));
	if (kickReactionAbilityBP.Succeeded()) kickReactionAbility = kickReactionAbilityBP.Class;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(FName("CharacterCapsule"));

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetVisibility(false);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetGenerateOverlapEvents(true);
	static ConstructorHelpers::FClassFinder<UZombieAnimInstance> animBP(TEXT("AnimBlueprint'/Game/_Assets/Animations/Zombies/AnimBP_Zombie.AnimBP_Zombie_C'"));
	if (animBP.Succeeded()) GetMesh()->SetAnimClass(animBP.Class);

	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	healthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));
	healthWidget->SetupAttachment(RootComponent);
	healthWidget->SetVisibility(false);
	healthWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	healthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	healthWidget->SetDrawSize(FVector2D(180.f, 30.f));
	static ConstructorHelpers::FClassFinder<UWidgetZombieHealth> healthWidgetBP(TEXT("WidgetBlueprint'/Game/_Assets/WidgetBlueprints/Main/WBP_ZombieHealthWidget.WBP_ZombieHealthWidget_C'"));
	if (healthWidgetBP.Succeeded()) healthWidget->SetWidgetClass(healthWidgetBP.Class);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> impactParticleRef(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Minion_melee/FX/Impacts/P_Minion_Impact_Default.P_Minion_Impact_Default'"));
	if (impactParticleRef.Succeeded()) impactParticle = impactParticleRef.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> deathSoundCueRef(TEXT("SoundCue'/Game/_Assets/Sounds/Character/Zombie/SC_Zombie_Death.SC_Zombie_Death'"));
	if (deathSoundCueRef.Succeeded()) deathSound = deathSoundCueRef.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> impactSoundCueRef(TEXT("SoundCue'/Game/_Assets/Sounds/Character/Zombie/SC_Zombie_Hit.SC_Zombie_Hit'"));
	if (impactSoundCueRef.Succeeded()) impactSound = impactSoundCueRef.Object;

}

void AZombieCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	asc->InitAbilityActorInfo(this, this);
	FGameplayAbilitySpec spec1(hitReactionAbility);
	asc->GiveAbility(spec1);
	FGameplayAbilitySpec spec2(kickReactionAbility);
	asc->GiveAbility(spec2);
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	if (PrimitiveComponent)
	{
		FBodyInstance* BodyInstance = PrimitiveComponent->GetBodyInstance();
		if (BodyInstance)
		{
			BodyInstance->MassScale = 0.0f;
			BodyInstance->bLockXRotation = true;
			BodyInstance->bLockYRotation = true;
			BodyInstance->bLockZRotation = true;
			BodyInstance->bLockXTranslation = true;
			BodyInstance->bLockYTranslation = true;
			BodyInstance->bLockZTranslation = true;
			BodyInstance->UpdateMassProperties();
		}
	}

	healthWidgetObject = Cast<UWidgetZombieHealth>(healthWidget->GetWidget());
	check(healthWidgetObject.IsValid());
	healthWidgetObject->InitHealthWidget(this);
}

void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (state == EZombieState::ATTACK)
	{
		if (IsValid(targetPlayer))
		{
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), (targetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation(), DeltaTime, 25.f));
		}
	}
}

void AZombieCharacter::ActivateActor()
{
	isActive = true;
	GetMesh()->SetCollisionProfileName(FName("ActivatedZombie"));
	GetMesh()->SetVisibility(true);
	healthWidgetObject->InitializeProgressBar();
}

void AZombieCharacter::DeactivateActor()
{
	isActive = false;
	SetActorLocation(FVector(0, 0, -3500));
	
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(FName("DeactivatedZombie"));
	GetMesh()->SetVisibility(false);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
}

bool AZombieCharacter::IsActorActivated()
{
	return isActive;
}

void AZombieCharacter::SetZombieInfo(const ZombieInfo& info)
{
	const int bitMax = static_cast<int>(ZIBT::MAX);
	for (int bit = 0; bit < bitMax; bit++)
	{
		if (info.recvInfoBitMask & (1 << bit))
		{
			ProcessZombieInfo(info, bit);
		}
	}
}

void AZombieCharacter::ProcessZombieInfo(const ZombieInfo& info, const int bitType)
{
	ZIBT type = static_cast<ZIBT>(bitType);
	switch (type)
	{
		case ZIBT::Location:
		{
			SetActorLocation(FVector(info.location.X, info.location.Y, GetActorLocation().Z));
			break;
		}
		case ZIBT::Rotation:
		{
			SetActorRotation(info.rotation);
			break;
		}
		case ZIBT::State:
		{
			SetZombieState(info.state);
			break;
		}
		case ZIBT::TargetNumber:
		{
			break;
		}
		case ZIBT::NextLocation:
		{
			SetNextLocation(info.nextLocation);
			break;
		}
	}
}

void AZombieCharacter::SetZombieState(const EZombieState newState)
{
	if (state != EZombieState::ATTACK && newState == EZombieState::ATTACK)
	{
		animInst->PlayAttackMontage(FMath::RandRange(1, 3));
	}
	else if (newState != EZombieState::CHASE)
	{
		GetWorldTimerManager().ClearTimer(movementUpdateTimer);
	}
	state = newState;
}

void AZombieCharacter::SetNextLocation(const FVector& nextLoc)
{
	if (nextLoc.X != nextPoint.X || nextLoc.Y != nextPoint.Y)
	{
		nextPoint = nextLoc;
		nextDirection = (nextPoint - GetActorLocation()).GetSafeNormal();
		const float dist = FVector::Distance(nextPoint, GetActorLocation());
		if (GetWorldTimerManager().IsTimerActive(movementUpdateTimer) == false && dist >= 1.f)
		{
			GetWorldTimerManager().SetTimer(movementUpdateTimer, this, &AZombieCharacter::StartMovementUpdate, interval, true);
		}
	}
}

void AZombieCharacter::UpdateHealth(const float newHealth)
{
	health = newHealth;
	onHealthChanged.ExecuteIfBound(health / maxHealth);
	healthWidget->SetVisibility(true);
	if (health)
	{
		GetWorldTimerManager().SetTimer(healthWidgetHideTimer, this, &AZombieCharacter::HideHealthWidget, 10.f);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(healthWidgetHideTimer);
	}
}

void AZombieCharacter::HideHealthWidget()
{
	healthWidget->SetVisibility(false);
}

void AZombieCharacter::UpdateMovement()
{
	const float maxStep = speed * interval;
	FVector nextLocation;
	if ((nextPoint - GetActorLocation()).Size() > maxStep)
	{
		nextLocation = GetActorLocation() + (nextDirection * speed * interval);
	}
	else
	{
		nextLocation = nextPoint;
	}
	VectorTruncate(nextLocation);
	SetActorLocation(FVector(nextLocation.X, nextLocation.Y, GetActorLocation().Z));
	FRotator nextRotation = FMath::RInterpTo(GetActorRotation(), nextDirection.Rotation(), GetWorld()->GetDeltaSeconds(), 15.f);
	SetActorRotation(FRotator(0.f, nextRotation.Yaw, 0.f));
}

void AZombieCharacter::FollowPath()
{
	const float dist = FVector::Distance(nextPoint, GetActorLocation());
	if (dist <= 1)
	{
		GetWorldTimerManager().ClearTimer(movementUpdateTimer);
	}
	UpdateMovement();
}

void AZombieCharacter::StartMovementUpdate()
{
	if (IsValid(targetPlayer))
	{
		const float dist = FVector::Distance(GetActorLocation(), targetPlayer->GetActorLocation());
		if (dist > 70.f && dist <= 1200.f)
		{
			FollowPath();
			return;
		}
	}
	GetWorldTimerManager().ClearTimer(movementUpdateTimer);
}

void AZombieCharacter::StartAttack()
{
	isAttackActivated = true;
}

void AZombieCharacter::EndAttack()
{
	isAttackActivated = false;
}

void AZombieCharacter::AttackFailed()
{
	TWeakObjectPtr<APlayerControllerMainMap> playerController = Cast<APlayerControllerMainMap>(targetPlayer->GetController());
	if (playerController.IsValid())
	{
		FHitResult hit;
		playerController->SendZombieHitsMe(number, false, hit);
	}
}

void AZombieCharacter::ActivateAttackTrace(const int attackAnimationType)
{
	if (isAttackActivated == false)
		return;

	const FVector socketLocation = GetMesh()->GetSocketLocation(GetSocketName(attackAnimationType));
	FHitResult hit;
	UKismetSystemLibrary::SphereTraceSingle
	(
		this,
		socketLocation,
		socketLocation + FVector(0, 0, 1),
		24,
		UEngineTypes::ConvertToTraceType(ECC_ZombieAttack),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		hit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.f
	);

	if (hit.bBlockingHit)
	{
		PlayHitEffect(hit);

		APlayerCharacter* player = Cast<APlayerCharacter>(hit.GetActor());
		if (IsValid(player) && player == targetPlayer)
		{
			FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(hit);
			FGameplayEventData payloadData;
			payloadData.TargetData.Add(targetData);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(hit.GetActor(), UD_EVENT_CHARACTER_HITREACTION, payloadData);

			TWeakObjectPtr<APlayerControllerMainMap> playerController = Cast<APlayerControllerMainMap>(player->GetController());
			if (playerController.IsValid())
			{
				playerController->SendZombieHitsMe(number, true, hit);
			}

			EndAttack();
		}
	}
}

void AZombieCharacter::PlayHitEffect(const FHitResult& hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, impactParticle, hit.ImpactPoint, hit.ImpactNormal.Rotation(), true);
	UGameplayStatics::PlaySoundAtLocation(this, impactSound, hit.ImpactPoint);
}

void AZombieCharacter::ZombieDead()
{
	UGameplayStatics::PlaySoundAtLocation(this, deathSound, GetActorLocation());
	state = EZombieState::IDLE;
	GetWorldTimerManager().ClearTimer(movementUpdateTimer);
	GetWorldTimerManager().SetTimer(deactivateDelayTimer, this, &AZombieCharacter::DeactivateAfterDelay, 3.f);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	healthWidget->SetVisibility(false);
}

void AZombieCharacter::DeactivateAfterDelay()
{
	DeactivateActor();
}

UAbilitySystemComponent* AZombieCharacter::GetAbilitySystemComponent() const
{
	return asc;
}


EZombieState AZombieCharacter::GetZombieState() const
{
	return state;
}

float AZombieCharacter::GetSpeed() const
{
	if (GetVelocity().Size2D())
	{
		return 300.f;
	}
	else
	{
		return 0.f;
	}
}

void AZombieCharacter::SetSkeletalMesh(USkeletalMesh* skeletalMesh)
{
	GetMesh()->SetSkeletalMesh(skeletalMesh);
	animInst = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
}

FName AZombieCharacter::GetSocketName(const int animationType)
{
	if (animationType == 1)
	{
		return FName("RightAttackSocket");
	}
	else if (animationType == 2)
	{
		return FName("LeftAttackSocket");
	}
	else if (animationType == 3)
	{
		return FName("RightAttackSocket");
	}
	else
	{
		return FName("");
	}
}

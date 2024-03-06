// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie/ZombieCharacter.h"
#include "Zombie/ZombieAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UntilDawn/UntilDawn.h"
#include "AIController.h"


AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AZombieAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::Spawned;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionProfileName(FName("DeactivatedZombie"));

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetVisibility(false);

	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset(TEXT("SkeletalMesh'/Game/Mixamo/zombies/Ch10_nonPBR.Ch10_nonPBR'"));
	if (skeletalMeshAsset.Succeeded()) { GetMesh()->SetSkeletalMesh(skeletalMeshAsset.Object); }

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> animBP(TEXT("AnimBlueprint'/Game/_Assets/Animations/Zombies/AnimBP_Zombie.AnimBP_Zombie_C'"));
	if (animBP.Succeeded()) GetMesh()->SetAnimClass(animBP.Class);
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombieCharacter::ActivateActor()
{
	isActive = true;
	GetCapsuleComponent()->SetCollisionProfileName(FName("ActivatedZombie"));
	GetMesh()->SetVisibility(true);
}

void AZombieCharacter::DeactivateActor()
{
	isActive = false;
	GetCapsuleComponent()->SetCollisionProfileName(FName("DeactivatedZombie"));
	GetMesh()->SetVisibility(false);
}

void AZombieCharacter::SetZombieState(const EZombieState newState)
{
	state = newState;
}

EZombieState AZombieCharacter::GetZombieState() const
{
	return state;
}

float AZombieCharacter::GetSpeed() const
{
	if (GetVelocity().Size2D()) return 300.f;
	else return 0.f;
}

void AZombieCharacter::SetPath(const vector<Pos>& path)
{
	pathToTarget = path;
	InitializePathStatus();
	for (Pos& pos : pathToTarget)
	{
		DrawDebugPoint(GetWorld(), FVector(pos.x, pos.y, 10.f), 7, FColor::Green, false, 2.f);
	}
}

void AZombieCharacter::InitializePathStatus()
{
	pathIdx = 0;
	nextPoint = FVector(pathToTarget[1].x, pathToTarget[1].y, GetActorLocation().Z);
	nextDirection = (nextPoint - GetActorLocation()).GetSafeNormal();
	GetWorldTimerManager().SetTimer(movementUpdateTimer, this, &AZombieCharacter::StartMovementUpdate, interval, true);
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
	SetActorLocation(nextLocation);
	PLOG(TEXT("%s"), *nextLocation.ToString());
}

void AZombieCharacter::FollowPath()
{
	const float dist = FVector::Distance(nextPoint, GetActorLocation());
	if (dist <= 50)
	{
		if (pathIdx < pathToTarget.size())
		{
			nextPoint = FVector(pathToTarget[pathIdx].x, pathToTarget[pathIdx].y, GetActorLocation().Z);
			nextDirection = (nextPoint - GetActorLocation()).GetSafeNormal();
			pathIdx++;
		}
	}
	UpdateMovement();
}

void AZombieCharacter::StartMovementUpdate()
{
	if (IsValid(targetPlayer))
	{
		const float dist = FVector::Distance(GetActorLocation(), targetPlayer->GetActorLocation());
		if (dist > 100.f && dist <= 1200.f)
		{
			FollowPath();			
			return;
		}
	}
	GetWorldTimerManager().ClearTimer(movementUpdateTimer);
}


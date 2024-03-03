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

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetVisibility(false);

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

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

EZombieState AZombieCharacter::GetZombieState() const
{
	return state;
}

float AZombieCharacter::GetSpeed() const
{
	if (GetVelocity().Size2D()) return 300.f;
	else return 0.f;
}

void AZombieCharacter::Move()
{
	if (bMoving == false)
	{
		AZombieAIController* C = Cast<AZombieAIController>(GetController());
		C->MoveToLocation(TargetLoc);
		bMoving = true;
	}
}

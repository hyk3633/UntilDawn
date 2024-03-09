

#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerAnimInst.h"
#include "Zombie/ZombieCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UntilDawn/UntilDawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "UntilDawn/UntilDawn.h"

#include "Zombie/ZombieCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::Disabled;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_ZombieAttack, ECollisionResponse::ECR_Block);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 400.0f;
	cameraBoom->bUsePawnControlRotation = true;

	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	followCamera->bUsePawnControlRotation = false;

	playerRange = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerRange"));
	playerRange->SetupAttachment(RootComponent);
	playerRange->SetCollisionObjectType(ECC_PlayerRange);
	playerRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	playerRange->SetSphereRadius(1024);
	playerRange->bHiddenInGame = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset(TEXT("SkeletalMesh'/Game/G2_SurvivalCharacters/Meshes/Characters/Combines/SK_Phong_Base.SK_Phong_Base'"));
	if (skeletalMeshAsset.Succeeded()) { GetMesh()->SetSkeletalMesh(skeletalMeshAsset.Object); }

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	static ConstructorHelpers::FClassFinder<UPlayerAnimInst> animBP(TEXT("AnimBlueprint'/Game/_Assets/Animations/Player/AnimBP_Player.AnimBP_Player_C'"));
	if (animBP.Succeeded()) GetMesh()->SetAnimClass(animBP.Class);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> obj_DefaultContext(TEXT("/Game/_Assets/Inputs/IMC_Defaults.IMC_Defaults"));
	if (obj_DefaultContext.Succeeded()) defaultMappingContext = obj_DefaultContext.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> obj_Jump(TEXT("/Game/_Assets/Inputs/Actions/IA_Jump.IA_Jump"));
	if (obj_Jump.Succeeded()) jumpAction = obj_Jump.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> obj_Move(TEXT("/Game/_Assets/Inputs/Actions/IA_Move.IA_Move"));
	if (obj_Move.Succeeded()) moveAction = obj_Move.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> obj_Look(TEXT("/Game/_Assets/Inputs/Actions/IA_Look.IA_Look"));
	if (obj_Look.Succeeded()) lookAction = obj_Look.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_Sprint(TEXT("/Game/_Assets/Inputs/Actions/IA_Sprint.IA_Sprint"));
	if (obj_Sprint.Succeeded()) sprintAction = obj_Sprint.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_LeftClick(TEXT("/Game/_Assets/Inputs/Actions/IA_LeftClick.IA_LeftClick"));
	if (obj_LeftClick.Succeeded()) leftClickAction = obj_LeftClick.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_LeftClickHold(TEXT("/Game/_Assets/Inputs/Actions/IA_LeftClickHold.IA_LeftClickHold"));
	if (obj_LeftClickHold.Succeeded()) leftClickHoldAction = obj_LeftClickHold.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_RightClick(TEXT("/Game/_Assets/Inputs/Actions/IA_RightClick.IA_RightClick"));
	if (obj_RightClick.Succeeded()) rightClickAction = obj_RightClick.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_RKey(TEXT("/Game/_Assets/Inputs/Actions/IA_RKey.IA_RKey"));
	if (obj_RKey.Succeeded()) rKeyAction = obj_RKey.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_RKeyHold(TEXT("/Game/_Assets/Inputs/Actions/IA_RKeyHold.IA_RKeyHold"));
	if (obj_RKeyHold.Succeeded()) rKeyHoldAction = obj_RKeyHold.Object;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	animInst = Cast<UPlayerAnimInst>(GetMesh()->GetAnimInstance());
	if (animInst) animInst->SetMyCharacter(this);
}

void APlayerCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
	myController = Cast<APlayerControllerMainMap>(newController);
	if (myController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(myController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
	}
	playerRange->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnPlayerRangeComponentBeginOverlap);
	playerRange->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnPlayerRangeComponentEndOverlap);
	OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::TakeAnyDamage);
	GetWorldTimerManager().SetTimer(overlappingZombieCheckTimer, this, &APlayerCharacter::OverlappingZombieCheck, 0.5f, true);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(playerInputComponent)) {
		EnhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::SprintEnd);
		EnhancedInputComponent->BindAction(leftClickAction, ETriggerEvent::Started, this, &APlayerCharacter::LeftClick);
		EnhancedInputComponent->BindAction(leftClickHoldAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LeftClickHold);
		EnhancedInputComponent->BindAction(leftClickAction, ETriggerEvent::Completed, this, &APlayerCharacter::LeftClickEnd);
		EnhancedInputComponent->BindAction(rightClickAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RightClick);
		EnhancedInputComponent->BindAction(rightClickAction, ETriggerEvent::Completed, this, &APlayerCharacter::RightClickEnd);
		EnhancedInputComponent->BindAction(rKeyAction, ETriggerEvent::Completed, this, &APlayerCharacter::RKeyPressed);
		EnhancedInputComponent->BindAction(rKeyHoldAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RKeyHold);
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

		const FVector rightDirection = FRotationMatrix(rotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(forwardDirection, movementVector.Y);
		AddMovementInput(rightDirection, movementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D lookAxisVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(-lookAxisVector.X);
		AddControllerPitchInput(lookAxisVector.Y);
		if (velocity.Size() == 0)
		{
			turnRight = (lookAxisVector.X < -0.3f ? true : false);
			turnLeft = (lookAxisVector.X > 0.3f ? true : false);
		}
		else
		{
			turnRight = false;
			turnLeft = false;
		}
	}
}

void APlayerCharacter::Sprint()
{
	if (isAbleShoot) return;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	if(myController)
		myController->SendPlayerInputAction(EPlayerInputs::Sprint);
}

void APlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 300;
	if (myController)
		myController->SendPlayerInputAction(EPlayerInputs::SprintEnd);
}

void APlayerCharacter::LeftClick()
{
	if (currentWeaponType == EWeaponType::AXE)
	{
		animInst->PlayAxeAttackMontage();
	}
	else if (currentWeaponType == EWeaponType::BOW)
	{
		animInst->PlayBowDrawMontage();
	}
	if (myController)
		myController->SendPlayerInputAction(EPlayerInputs::LeftClick);
}

void APlayerCharacter::LeftClickHold()
{
	if (currentWeaponType == EWeaponType::BOW)
	{
		isAbleShoot = true;
		GetCharacterMovement()->MaxWalkSpeed = 300;
	}
	if (myController)
		myController->SendPlayerInputAction(EPlayerInputs::LeftClickHold);
}

void APlayerCharacter::LeftClickEnd()
{
	if (currentWeaponType == EWeaponType::BOW)
	{
		if (isAbleShoot == false) return;
		isAbleShoot = false;
		animInst->PlayBowShootMontage();
		shootPower = 0;
	}
	if (myController)
		myController->SendPlayerInputAction(EPlayerInputs::LeftClickEnd);
}

void APlayerCharacter::RightClick()
{
	if (currentWeaponType == EWeaponType::AXE)
	{
		rightClick = true;
	}
	if (myController)
		myController->SendPlayerInputAction(EPlayerInputs::RightClick);
}

void APlayerCharacter::RightClickEnd()
{
	if (currentWeaponType == EWeaponType::AXE)
	{
		rightClick = false;
	}
	if (myController)
		myController->SendPlayerInputAction(EPlayerInputs::RightClickEnd);
}

void APlayerCharacter::RKeyPressed()
{
	if (currentWeaponType != EWeaponType::DEFAULT) return;
	animInst->PlayWeaponArmMontage(EWeaponType::AXE);
	currentWeaponType = EWeaponType::AXE;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	if (myController)
		myController->SendPlayerInputAction(EPlayerInputs::RKeyPressed);
}

void APlayerCharacter::RKeyHold()
{
	if (currentWeaponType == EWeaponType::DEFAULT) return;
	animInst->PlayWeaponDisarmMontage(currentWeaponType);
	currentWeaponType = EWeaponType::DEFAULT;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	if (myController)
		myController->SendPlayerInputAction(EPlayerInputs::RKeyHold);
}

void APlayerCharacter::OnPlayerRangeComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FScopeLock Lock(&criticalSection);
	AZombieCharacter* zombie = Cast<AZombieCharacter>(OtherActor);
	if (zombie->GetZombieState() != EZombieState::IDLE) return;
	if (IsValid(zombie))
	{
		if (IsZombieCanSeeMe(zombie))
			zombiesWhoSawMe.Add(zombie);
		else
			overlappingZombies.Add(zombie->GetNumber(), zombie);
	}
}

void APlayerCharacter::OnPlayerRangeComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FScopeLock Lock(&criticalSection);
	AZombieCharacter* zombie = Cast<AZombieCharacter>(OtherActor);
	if (IsValid(zombie))
	{
		if (overlappingZombies.Find(zombie->GetNumber()))
		{
			overlappingZombies.Remove(zombie->GetNumber());
		}
	}
}

bool APlayerCharacter::IsZombieCanSeeMe(AActor* zombie)
{
	FVector ToTarget = GetActorLocation() - zombie->GetActorLocation();
	ToTarget.Normalize();
	const float AngleDegree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ToTarget, zombie->GetActorForwardVector())));
	if (AngleDegree < 60.0) return true;
	else return false;
}

void APlayerCharacter::OverlappingZombieCheck()
{
	FScopeLock Lock(&criticalSection);
	if (overlappingZombies.Num())
	{
		for (auto& kv : overlappingZombies)
		{
			if (IsValid(kv.Value))
			{
				if (IsZombieCanSeeMe(kv.Value))
				{
					zombiesWhoSawMe.Add(kv.Value);
					removePendingQ.Enqueue(kv.Value);
				}
			}
			else
			{
				removePendingQ.Enqueue(kv.Value);
			}
		}
		while (!removePendingQ.IsEmpty())
		{
			AZombieCharacter* zombie;
			removePendingQ.Dequeue(zombie);
			overlappingZombies.Remove(zombie->GetNumber());
		}
	}
}

void APlayerCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	speed = GetVelocity().Size2D();
	velocity = GetVelocity();

	direction = UKismetAnimationLibrary::CalculateDirection(velocity, GetActorForwardVector().Rotation());
	pitch = GetBaseAimRotation().Pitch;
	if (pitch >= 180.f) pitch -= 360.f;

	if (isAbleShoot)
		shootPower = FMath::Max(shootPower + deltaTime, 10.f);
}

void APlayerCharacter::UpdatePlayerInfo()
{
	FVector&& location = std::move(GetActorLocation());
	FRotator&& rotation = std::move(GetActorRotation());

	myInfo.characterInfo.vectorX = location.X;
	myInfo.characterInfo.vectorY = location.Y;
	myInfo.characterInfo.vectorZ = location.Z;
	myInfo.characterInfo.velocityX = velocity.X;
	myInfo.characterInfo.velocityY = velocity.Y;
	myInfo.characterInfo.velocityZ = velocity.Z;
	myInfo.characterInfo.pitch = rotation.Pitch;
	myInfo.characterInfo.yaw = rotation.Yaw;
	myInfo.characterInfo.roll = rotation.Roll;

	FScopeLock Lock(&criticalSection);

	myInfo.zombiesWhoSawMe.clear();
	if (!zombiesWhoSawMe.IsEmpty())
	{
		for (AZombieCharacter* zombie : zombiesWhoSawMe)
		{
			myInfo.zombiesWhoSawMe.push_back(zombie->GetNumber());
		}
		if (myInfo.zombiesWhoSawMe.size())
		{
			myInfo.isZombiesSawMe = true;
		}
		zombiesWhoSawMe.Reset(3);
	}
	else
	{
		myInfo.isZombiesSawMe = false;
	}
}

const bool APlayerCharacter::GetIsFalling() const
{
	return GetMovementComponent()->IsFalling();
}

void APlayerCharacter::DoPlayerInputAction(const int inputType)
{
	if (inputType == 0) return;
	switch (static_cast<EPlayerInputs>(inputType))
	{
	case EPlayerInputs::Sprint:
		Sprint();
		break;
	case EPlayerInputs::SprintEnd:
		SprintEnd();
		break;
	case EPlayerInputs::LeftClick:
		LeftClick();
		break;
	case EPlayerInputs::LeftClickHold:
		LeftClickHold();
		break;
	case EPlayerInputs::LeftClickEnd:
		LeftClickEnd();
		break;
	case EPlayerInputs::RightClick:
		RightClick();
		break;
	case EPlayerInputs::RightClickEnd:
		RightClickEnd();
		break;
	case EPlayerInputs::RKeyPressed:
		RKeyPressed();
		break;
	case EPlayerInputs::RKeyHold:
		RKeyHold();
		break;
	}
}

void APlayerCharacter::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	AZombieCharacter* zombie = Cast<AZombieCharacter>(DamageCauser);
	if (IsValid(zombie))
	{
		PLOG(TEXT("%d zombie %s hits me"), zombie->GetNumber(), *zombie->GetName());
	}
}


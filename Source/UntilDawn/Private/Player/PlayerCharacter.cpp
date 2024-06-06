

#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerAnimInst.h"
#include "UI/Main/HUDMainMap.h"
#include "UI/Main/WidgetPlayerHealth.h"
#include "Zombie/ZombieCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UntilDawn/UntilDawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "KismetAnimationLibrary.h"
#include "UntilDawn/UntilDawn.h"
#include "Item/ItemObject.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject/ItemPermanent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "AbilitySystemComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::Disabled;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(FName("RemotePlayer"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset(TEXT("SkeletalMesh'/Game/G2_SurvivalCharacters/Meshes/Characters/Combines/SK_Phong_Base.SK_Phong_Base'"));
	if (skeletalMeshAsset.Succeeded()) { GetMesh()->SetSkeletalMesh(skeletalMeshAsset.Object); }

	static ConstructorHelpers::FClassFinder<UPlayerAnimInst> animBP(TEXT("AnimBlueprint'/Game/_Assets/Animations/Player/AnimBP_Player.AnimBP_Player_C'"));
	if (animBP.Succeeded()) GetMesh()->SetAnimClass(animBP.Class);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	asc = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->SetRelativeLocation(FVector(0.f, 20.f, 50.f));
	cameraBoom->SetRelativeRotation(FRotator(10.f, 0.f, 0.f));
	cameraBoom->TargetArmLength = 250.0f;
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

	HeadMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Parts"));
	HeadMeshComponent->SetupAttachment(GetMesh());
	HeadMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadMeshComponent->SetLeaderPoseComponent(GetMesh());

	TopMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Top Parts"));
	TopMeshComponent->SetupAttachment(GetMesh());
	TopMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TopMeshComponent->SetLeaderPoseComponent(GetMesh());

	BottomMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bottom Parts"));
	BottomMeshComponent->SetupAttachment(GetMesh());
	BottomMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BottomMeshComponent->SetLeaderPoseComponent(GetMesh());

	FootMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Foot Parts"));
	FootMeshComponent->SetupAttachment(GetMesh());
	FootMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FootMeshComponent->SetLeaderPoseComponent(GetMesh());

	healthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));
	healthWidget->SetupAttachment(RootComponent);
	healthWidget->SetVisibility(false);
	healthWidget->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	healthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	healthWidget->SetDrawSize(FVector2D(250.f, 70.f));
	static ConstructorHelpers::FClassFinder<UWidgetPlayerHealth> healthWidgetBP(TEXT("WidgetBlueprint'/Game/_Assets/WidgetBlueprints/Main/WBP_PlayerHealthWidget.WBP_PlayerHealthWidget_C'"));
	if (healthWidgetBP.Succeeded()) healthWidget->SetWidgetClass(healthWidgetBP.Class);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> obj_DefaultContext(TEXT("/Game/_Assets/Inputs/IMC_DefaultsCharacter.IMC_DefaultsCharacter"));
	if (obj_DefaultContext.Succeeded()) defaultMappingContext = obj_DefaultContext.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> obj_Jump(TEXT("/Game/_Assets/Inputs/Actions/IA_Jump.IA_Jump"));
	if (obj_Jump.Succeeded()) jumpAction = obj_Jump.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> obj_Move(TEXT("/Game/_Assets/Inputs/Actions/IA_Move.IA_Move"));
	if (obj_Move.Succeeded()) moveAction = obj_Move.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> obj_Look(TEXT("/Game/_Assets/Inputs/Actions/IA_Look.IA_Look"));
	if (obj_Look.Succeeded()) lookAction = obj_Look.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> obj_Sprint(TEXT("/Game/_Assets/Inputs/Actions/IA_Sprint.IA_Sprint"));
	if (obj_Sprint.Succeeded()) sprintAction = obj_Sprint.Object;
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	asc->InitAbilityActorInfo(this, this);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	animInst = Cast<UPlayerAnimInst>(GetMesh()->GetAnimInstance());
	if (animInst)
	{
		animInst->InitAnimInst(this);
		animInst->DMontageEnded.BindUFunction(this, FName("WrestlingEnd"));
	}

	for (const auto& ability : defaultAbilities)
	{
		FGameplayAbilitySpec spec(ability);
		asc->GiveAbility(spec);
	}

	for (const auto& ability : inputAbilities)
	{
		FGameplayAbilitySpec spec(ability.Value);
		spec.InputID = StaticCast<uint8>(ability.Key);
		asc->GiveAbility(spec);
	}
}

void APlayerCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	TWeakObjectPtr<APlayerController> playerController = Cast<APlayerController>(newController);
	if (playerController.IsValid())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
	}

	GetCapsuleComponent()->SetCollisionProfileName(FName("LocalPlayer"));
	playerRange->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnPlayerRangeComponentBeginOverlap);
	playerRange->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnPlayerRangeComponentEndOverlap);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(playerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(jumpAction,		ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(jumpAction,		ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(moveAction,		ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(lookAction,		ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(sprintAction,	ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);	
		EnhancedInputComponent->BindAction(sprintAction,	ETriggerEvent::Completed, this, &APlayerCharacter::SprintEnd);	
	}
}

bool APlayerCharacter::CheckAbleInput()
{
	if (bWrestling) return false;
	else return true;
}

void APlayerCharacter::Jump()
{
	if (CheckAbleInput() == false)
		return;
	ACharacter::Jump();
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (CheckAbleInput() == false)
		return;

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
	if (CheckAbleInput() == false)
		return;

	GetCharacterMovement()->MaxWalkSpeed = 600;
}

void APlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 300;
}

bool APlayerCharacter::LeftClick(const EWeaponType weaponType)
{
	if (CheckAbleInput() == false)
		return false;

	if (weaponType != EWeaponType::NONE)
	{
		if (weaponType == EWeaponType::BOW)
		{
			bowStatus |= (1 << StaticCast<uint8>(EBowStatus::Loaded));
		}
		animInst->PlayLeftClickMontage(weaponType);
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayerCharacter::LeftClickHold(const EWeaponType weaponType)
{
	if (CheckAbleInput() == false)
		return false;

	if (weaponType == EWeaponType::BOW)
	{
		bowStatus |= (1 << StaticCast<uint8>(EBowStatus::Drawed));
		GetCharacterMovement()->MaxWalkSpeed = 300;
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayerCharacter::LeftClickEnd(const EWeaponType weaponType)
{
	if (weaponType == EWeaponType::BOW)
	{
		if (bowStatus == StaticCast<uint8>(EBowStatus::Full))
		{
			animInst->PlayBowShootMontage();
			shootPower = 0;
			bowStatus = 0;
			return true;
		}
		bowStatus = 0;
	}
	
	return false;
}

bool APlayerCharacter::RightClick(const EWeaponType weaponType)
{
	if (CheckAbleInput() == false)
		return false;

	if (weaponType == EWeaponType::AXE)
	{
		rightClick = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayerCharacter::RightClickEnd(const EWeaponType weaponType)
{
	if (CheckAbleInput() == false)
		return false;

	if (weaponType == EWeaponType::AXE)
	{
		rightClick = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayerCharacter::ArmWeapon(TWeakObjectPtr<AItemBase> itemActor)
{
	if (CheckAbleInput() == false)
		return false;

	armedWeapon = itemActor;

	TWeakObjectPtr<UItemPermanent> permanentItemObj = Cast<UItemPermanent>(itemActor->GetItemObject());
	currentWeaponType = StaticCast<EWeaponType>(permanentItemObj->GetItemSubType());

	AttachItemActor(itemActor);

	asc->AddLooseGameplayTags(itemActor->GetGameplayTags());

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	return true;
}

bool APlayerCharacter::HKeyPressed()
{
	return CheckAbleInput();
}

void APlayerCharacter::SuccessToBlocking()
{
	SetWrestlingOff();
	PlayPushingZombieMontage(true);
}

void APlayerCharacter::OnPlayerRangeComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZombieCharacter* zombie = Cast<AZombieCharacter>(OtherActor);
	if (IsValid(zombie) && zombie->GetZombieState() == EZombieState::IDLE)
	{
		DZombieInRange.ExecuteIfBound(zombie->GetNumber());
	}
}

void APlayerCharacter::OnPlayerRangeComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AZombieCharacter* zombie = Cast<AZombieCharacter>(OtherActor);
	if (IsValid(zombie))
	{
		DZombieOutRange.ExecuteIfBound(zombie->GetNumber());
	}
}

void APlayerCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	speed = GetVelocity().Size2D();
	velocity = GetVelocity();

	direction = UKismetAnimationLibrary::CalculateDirection(velocity, GetActorForwardVector().Rotation());
	pitch = GetBaseAimRotation().Pitch;
	if (pitch >= 180.f)
	{
		pitch -= 360.f;
	}
	yaw = GetBaseAimRotation().Yaw;
	if (yaw >= 180.f)
	{
		yaw -= 360.f;
	}

	if (bowStatus)
	{
		shootPower = FMath::Max(shootPower + deltaTime * 2.f, 10.f);
	}

	if (bset)
	{
		FVector de = FMath::VInterpConstantTo(GetActorLocation(), nextLocation, deltaTime, 150);
		VectorTruncate(de);
		SetActorLocation(de);
	}
}

void APlayerCharacter::UpdatePlayerInfo()
{
	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();

	myInfo.vectorX = location.X;
	myInfo.vectorY = location.Y;
	myInfo.vectorZ = location.Z;
	myInfo.velocityX = velocity.X;
	myInfo.velocityY = velocity.Y;
	myInfo.velocityZ = velocity.Z;
	myInfo.pitch = rotation.Pitch;
	myInfo.yaw = rotation.Yaw;
	myInfo.roll = rotation.Roll;
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return asc;
}

void APlayerCharacter::SetPlayerIDAndNumber(const FString& id, const int number)
{
	playerID = id;
	playerNumber = number;
}

const bool APlayerCharacter::GetIsFalling() const
{
	return GetMovementComponent()->IsFalling();
}

EWeaponType APlayerCharacter::GetCurrentWeaponType() const
{
	return currentWeaponType;
}

void APlayerCharacter::DoPlayerInputAction(const int inputType, const int weaponType)
{
	if (inputType == 0) 
		return;
	const EWeaponType eWeaponType = static_cast<EWeaponType>(weaponType);
	switch (static_cast<EPlayerInputs>(inputType))
	{
	case EPlayerInputs::LeftClick:
		LeftClick(eWeaponType);
		break;
	case EPlayerInputs::LeftClickHold:
		LeftClickHold(eWeaponType);
		break;
	case EPlayerInputs::LeftClickEnd:
		LeftClickEnd(eWeaponType);
		break;
	case EPlayerInputs::RightClick:
		RightClick(eWeaponType);
		break;
	case EPlayerInputs::RightClickEnd:
		RightClickEnd(eWeaponType);
		break;
	}
}

void APlayerCharacter::SetWrestlingOn()
{
	bWrestling = true;
}

void APlayerCharacter::SetWrestlingOff()
{
	bWrestling = false;
}

void APlayerCharacter::PlayPushingZombieMontage(const bool isBlocking)
{
	animInst->PlayWrestlingMontage(isBlocking);
}

void APlayerCharacter::FailedToResist()
{
	SetWrestlingOff();
	PlayPushingZombieMontage(false);
}

void APlayerCharacter::WrestlingEnd()
{
	
}

void APlayerCharacter::PlayerDead()
{
	GetCapsuleComponent()->SetCollisionProfileName(FName("DeadPlayer"));
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	healthWidget->SetVisibility(false);
	InitializePlayerInfo();
}

void APlayerCharacter::InitializePlayerInfo()
{
	shootPower = false;
	bWrestling = false;
}

void APlayerCharacter::PlayerRespawn(const bool isLocalPlayer)
{
	if (isLocalPlayer)
	{
		GetCapsuleComponent()->SetCollisionProfileName(FName("LocalPlayer"));
	}
	else
	{
		GetCapsuleComponent()->SetCollisionProfileName(FName("RemotePlayer"));
	}
	GetCapsuleComponent()->SetRelativeRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(FName("NormalMesh"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetVisibility(true);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
}

void APlayerCharacter::DeadReckoningMovement(const FVector& lastLocation, const FVector& lastVelocity, const double ratency)
{
	nextLocation = lastLocation + ratency * lastVelocity;
	bset = true;
}

void APlayerCharacter::SetHealth(const float newHealth)
{
	health = FMath::Min(newHealth, maxHealth);
	healthWidgetObject->SetProgressPercentage(GetHealthPercentage());
}

float APlayerCharacter::GetHealthPercentage()
{
	return health / maxHealth;
}

void APlayerCharacter::RecoverHealth(const float recoveryAmount)
{
	health = FMath::Clamp(health + recoveryAmount, 0.f, maxHealth);
}

void APlayerCharacter::AttachItemActor(TWeakObjectPtr<AItemBase> item)
{
	const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName(item->GetSocketName());
	socket->AttachActor(item.Get(), GetMesh());
}

void APlayerCharacter::EquipItem(TWeakObjectPtr<AItemBase> item, const int8 slotNumber)
{
	item->ActivateEquipMode(item->GetItemType());

	TWeakObjectPtr<UItemPermanent> permanentItemObj = Cast<UItemPermanent>(item->GetItemObject());
	permanentItemObj->SetEquippedSlotNumber(slotNumber);

	if (item->GetItemType() == EItemMainType::ArmourItem)
	{
		EArmourSlot armourType = StaticCast<EArmourSlot>(permanentItemObj->GetItemSubType());
		switch (armourType)
		{
		case EArmourSlot::Head:
			HeadMeshComponent->SetSkeletalMesh(item->GetItemObject()->GetSkeletalMesh());
			break;
		case EArmourSlot::Top:
			TopMeshComponent->SetSkeletalMesh(item->GetItemObject()->GetSkeletalMesh());
			break;
		case EArmourSlot::Bottom:
			BottomMeshComponent->SetSkeletalMesh(item->GetItemObject()->GetSkeletalMesh());
			break;
		case EArmourSlot::Foot:
			FootMeshComponent->SetSkeletalMesh(item->GetItemObject()->GetSkeletalMesh());
			break;
		}
	}
	else
	{
		FName socketName;
		if (slotNumber == 0)
		{
			socketName = "Ranged1_Back";
		}
		else if (slotNumber == 1)
		{
			socketName = "Ranged2_Back";
		}
		else
		{
			socketName = "Melee_Back";
		}
		const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName(socketName);
		socket->AttachActor(item.Get(), GetMesh());
	}
}

void APlayerCharacter::UnEquipItem(TWeakObjectPtr<AItemBase> item)
{
	TWeakObjectPtr<UItemPermanent> permanentItemObj = Cast<UItemPermanent>(item->GetItemObject());
	permanentItemObj->SetEquippedSlotNumber(-1);

	if (item->GetItemType() == EItemMainType::ArmourItem)
	{
		EArmourSlot armourType = StaticCast<EArmourSlot>(permanentItemObj->GetItemSubType());
		switch (armourType)
		{
		case EArmourSlot::Head:
			HeadMeshComponent->SetSkeletalMesh(nullptr);
			break;
		case EArmourSlot::Top:
			TopMeshComponent->SetSkeletalMesh(nullptr);
			break;
		case EArmourSlot::Bottom:
			BottomMeshComponent->SetSkeletalMesh(nullptr);
			break;
		case EArmourSlot::Foot:
			FootMeshComponent->SetSkeletalMesh(nullptr);
			break;
		}
	}
	else
	{
		if (item == armedWeapon)
		{
			DisarmWeapon();
		}
		item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void APlayerCharacter::ShowHealthWidget()
{
	if (GetWorldTimerManager().IsTimerActive(healthWidgetDeacitvateTimer))
	{
		GetWorldTimerManager().ClearTimer(healthWidgetDeacitvateTimer);
	}
	GetWorldTimerManager().SetTimer(healthWidgetDeacitvateTimer, this, &APlayerCharacter::HideHealthWidget, 5.f);
	healthWidget->SetVisibility(true);
}

void APlayerCharacter::InitializeHealthWidget()
{
	healthWidgetObject = Cast<UWidgetPlayerHealth>(healthWidget->GetWidget());
	check(healthWidgetObject.IsValid());
	healthWidgetObject->InitHealthWidget(playerID, GetHealthPercentage());
}

void APlayerCharacter::SetMaxHealth(const int newHealth)
{
	maxHealth = newHealth;
}

void APlayerCharacter::HideHealthWidget()
{
	healthWidget->SetVisibility(false);
}

void APlayerCharacter::ActivateAbility(TSubclassOf<UGameplayAbility> ability, const EInputType inputType)
{
	FGameplayAbilitySpec* specPtr = asc->FindAbilitySpecFromClass(ability);
	if (specPtr)
	{
		specPtr->InputID = StaticCast<uint8>(inputType);
		if (specPtr->IsActive())
		{
			if (inputType == EInputType::LeftClick_Released || inputType == EInputType::RightClick_Released)
			{
				asc->AbilitySpecInputReleased(*specPtr);
			}
			else
			{
				specPtr->InputPressed = true;
				asc->AbilitySpecInputPressed(*specPtr);
			}
		}
		else
		{
			if (inputType != EInputType::LeftClick_Released && inputType != EInputType::RightClick_Released)
			{
				asc->TryActivateAbility(specPtr->Handle);
			}
		}
	}
	else
	{
		FGameplayAbilitySpec spec(ability);
		spec.InputID = StaticCast<uint8>(inputType);
		asc->GiveAbility(spec);
		asc->TryActivateAbilityByClass(ability);
	}
}

bool APlayerCharacter::TryActivateAbility(const EInputType inputType)
{
	FGameplayAbilitySpec* spec = asc->FindAbilitySpecFromInputID(StaticCast<uint8>(inputType));
	if (spec)
	{
		const bool result = asc->TryActivateAbility(spec->Handle);
		return result;
	}
	return false;
}

void APlayerCharacter::ActivateInputInterval()
{
	bInputInterval = true;
}

void APlayerCharacter::DeactivateInputInterval()
{
	bInputInterval = false;
}

void APlayerCharacter::ActivateAiming()
{
	bAiming = true;
}

void APlayerCharacter::DeactivateAiming()
{
	bAiming = false;
}

// 애님 노티파이 호출
void APlayerCharacter::AttachDisarmedWeaponToBack()
{
	TWeakObjectPtr<UItemPermanent> permanentItemObj = Cast<UItemPermanent>(armedWeapon->GetItemObject());
	EquipItem(armedWeapon, permanentItemObj->GetEquippedSlotNumber());
	armedWeapon.Reset();
	currentWeaponType = EWeaponType::NONE;
}

void APlayerCharacter::ChangeWeapon(TWeakObjectPtr<AItemBase> changedWeaponActor)
{
	asc->RemoveLooseGameplayTags(armedWeapon->GetGameplayTags());
	AttachDisarmedWeaponToBack();
	armedWeapon = changedWeaponActor;
	ArmWeapon(armedWeapon);
}

TWeakObjectPtr<AItemBase> APlayerCharacter::GetArmedWeapon() const
{
	return armedWeapon;
}

bool APlayerCharacter::DisarmWeapon()
{
	if (CheckAbleInput() == false)
		return false;

	asc->RemoveLooseGameplayTags(armedWeapon->GetGameplayTags());

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	return true;
}

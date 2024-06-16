

#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerAnimInst.h"
#include "Player/RemotePlayerAIController.h"
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
#include "Tag/UntilDawnGameplayTags.h"
#include "Sound/SoundCue.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ARemotePlayerAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(FName("CharacterCapsule"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(FName("RemotePlayer"));

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

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->SetRelativeLocation(FVector(0.f, 20.f, 80.f));
	cameraBoom->SetRelativeRotation(FRotator(10.f, 0.f, 0.f));
	cameraBoom->TargetArmLength = 250.0f;
	cameraBoom->bUsePawnControlRotation = true;

	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	followCamera->bUsePawnControlRotation = false;

	playerRange = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerRange"));
	playerRange->SetupAttachment(RootComponent);
	playerRange->SetCollisionProfileName(FName("PlayerRange"));
	playerRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	playerRange->SetSphereRadius(1024);
	playerRange->bHiddenInGame = true;

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

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	animInst = Cast<UPlayerAnimInst>(GetMesh()->GetAnimInstance());
	if (animInst)
	{
		animInst->InitAnimInst(this);
	}
}

void APlayerCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	IAbilitySystemInterface* ascOwnerController = CastChecked<IAbilitySystemInterface>(newController);
	asc = ascOwnerController->GetAbilitySystemComponent();
	asc->InitAbilityActorInfo(newController, this);
	for (const auto& ability : defaultAbilities)
	{
		FGameplayAbilitySpec spec(ability);
		asc->GiveAbility(spec);
	}
	for (const auto& ability : inputAbilities)
	{
		if (asc->FindAbilitySpecFromClass(ability.Value) == nullptr)
		{
			FGameplayAbilitySpec spec(ability.Value);
			asc->GiveAbility(spec);
		}
	}

	TWeakObjectPtr<APlayerController> playerController = Cast<APlayerController>(newController);
	if (playerController.IsValid())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
		playerRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		isLocal = true;
	}

	GetMesh()->SetCollisionProfileName(FName("LocalPlayer"));
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
	}
}

void APlayerCharacter::Jump()
{
	ACharacter::Jump();
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

	velocity = GetVelocity();
	direction = UKismetAnimationLibrary::CalculateDirection(velocity, GetActorForwardVector().Rotation());

	if (isLocal)
	{
		SetTargetSpeed(velocity.Length());
	}
	SetPitchAndYaw(deltaTime);
}

void APlayerCharacter::SetTargetSpeed(const float speed)
{
	if (speed > 300.f)
	{
		targetSpeed = 600.f;
	}
	else if (speed > 0.f)
	{
		targetSpeed = 300.f;
	}
	else
	{
		targetSpeed = 0.f;
	}
}

void APlayerCharacter::SetPitchAndYaw(float deltaTime)
{
	FRotator deltaRotation = (GetControlRotation() - GetActorRotation());
	deltaRotation.Normalize();
	pitch = FMath::ClampAngle(deltaRotation.Pitch, -90, 90);
	yaw = FMath::ClampAngle(deltaRotation.Yaw, -90, 90);
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return asc;
}

bool APlayerCharacter::TryActivateWeaponAbility(const EInputType inputType)
{
	if (armedWeapon.IsValid())
	{
		TSubclassOf<UGameplayAbility> ability = armedWeapon->GetItemObject()->GetAbility(inputType);
		if (ability)
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
			return true;
		}
	}
	return false;
}

bool APlayerCharacter::TryActivateInputAbility(const EInputType inputType)
{
	FGameplayAbilitySpec* spec = asc->FindAbilitySpecFromClass(inputAbilities[inputType]);
	spec->InputID = StaticCast<uint8>(inputType);
	if (spec)
	{
		if (spec->IsActive())
		{
			asc->AbilitySpecInputReleased(*spec);
			return true;
		}
		else
		{
			const bool result = asc->TryActivateAbility(spec->Handle);
			return result;
		}
	}
	return false;
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

void APlayerCharacter::AttachItemActor(TWeakObjectPtr<AItemBase> item)
{
	const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName(item->GetSocketName());
	socket->AttachActor(item.Get(), GetMesh());
}

bool APlayerCharacter::ArmWeapon(TWeakObjectPtr<AItemBase> itemActor)
{
	armedWeapon = itemActor;

	TWeakObjectPtr<UItemPermanent> permanentItemObj = Cast<UItemPermanent>(itemActor->GetItemObject());
	currentWeaponType = StaticCast<EWeaponType>(permanentItemObj->GetItemSubType());

	AttachItemActor(itemActor);

	asc->AddLooseGameplayTags(itemActor->GetGameplayTags());

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	return true;
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

bool APlayerCharacter::DisarmWeapon()
{
	asc->RemoveLooseGameplayTags(armedWeapon->GetGameplayTags());

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	return true;
}

void APlayerCharacter::ChangeWeapon(TWeakObjectPtr<AItemBase> changedWeaponActor)
{
	asc->RemoveLooseGameplayTags(armedWeapon->GetGameplayTags());
	AttachDisarmedWeaponToBack();
	armedWeapon = changedWeaponActor;
	ArmWeapon(armedWeapon);
}

// 애님 노티파이 호출
void APlayerCharacter::AttachDisarmedWeaponToBack()
{
	TWeakObjectPtr<UItemPermanent> permanentItemObj = Cast<UItemPermanent>(armedWeapon->GetItemObject());
	EquipItem(armedWeapon, permanentItemObj->GetEquippedSlotNumber());
	armedWeapon.Reset();
	currentWeaponType = EWeaponType::NONE;
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

void APlayerCharacter::InitializeHealthWidget()
{
	healthWidgetObject = Cast<UWidgetPlayerHealth>(healthWidget->GetWidget());
	check(healthWidgetObject.IsValid());
	healthWidgetObject->InitHealthWidget(playerID, GetHealthPercentage());
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

void APlayerCharacter::RecoverHealth(const float recoveryAmount)
{
	health = FMath::Clamp(health + recoveryAmount, 0.f, maxHealth);
}

void APlayerCharacter::HideHealthWidget()
{
	healthWidget->SetVisibility(false);
}

void APlayerCharacter::SetHealth(const float newHealth)
{
	health = FMath::Min(newHealth, maxHealth);
	healthWidgetObject->SetProgressPercentage(GetHealthPercentage());
}

void APlayerCharacter::SetMaxHealth(const int newHealth)
{
	maxHealth = newHealth;
}

float APlayerCharacter::GetHealthPercentage()
{
	return health / maxHealth;
}

void APlayerCharacter::PlayerDead()
{
	UGameplayStatics::PlaySoundAtLocation(this, deathSound, GetActorLocation());
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	playerRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	healthWidget->SetVisibility(false);
}

void APlayerCharacter::PlayerRespawn()
{
	GetCapsuleComponent()->SetRelativeRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
	if (isLocal)
	{
		playerRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	const FName profileName = isLocal ? FName("LocalPlayer") : FName("RemotePlayer");
	GetMesh()->SetCollisionProfileName(profileName);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetVisibility(true);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
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

void APlayerCharacter::SetPitch(const float newPitch)
{
	pitch = newPitch;
}

bool APlayerCharacter::IsWrestling()
{
	return asc->HasMatchingGameplayTag(UD_CHARACTER_STATE_WRESTLING);
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




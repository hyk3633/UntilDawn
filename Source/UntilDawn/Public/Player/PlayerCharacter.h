// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "../Enums/WeaponType.h"
#include "../Enums/ArmourSlot.h"
#include "../Enums/InputType.h"
#include "Structs/CharacterInfo.h"
#include "Structs/Tile.h"
#include "AbilitySystemInterface.h"
#include "PlayerCharacter.generated.h"

class UAbilitySystemComponent;
class APlayerControllerMainMap;
class AHUDMainMap;
class UPlayerAnimInst;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class USphereComponent;
class AZombieCharacter;
class UItemObject;
class AItemBase;
class AItemMeleeWeapon;
class UWidgetComponent;
class UWidgetPlayerHealth;
class UItemPermanent;
class UGameplayAbility;
class UPlayerAttributeSet;
class USoundCue;

DECLARE_DELEGATE_OneParam(DelegateZombieInRange, int zombieNumber);
DECLARE_DELEGATE_OneParam(DelegateZombieOutRange, int zombieNumber);

enum class EBowStatus : uint8
{
	Loaded,
	Drawed,
	Full = 3,
};


UCLASS()
class UNTILDAWN_API APlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	DelegateZombieInRange DZombieInRange;
	DelegateZombieInRange DZombieOutRange;

protected:

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* newController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

	void Jump();

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

public:

	bool ArmWeapon(TWeakObjectPtr<AItemBase> itemActor);

	bool HKeyPressed();

protected:

	UFUNCTION()
	void OnPlayerRangeComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerRangeComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void Tick(float deltaTime) override;

	void SetPitchAndYaw(float deltaTime);

	void UpdatePlayerInfo();

	void SetPlayerIDAndNumber(const FString& id, const int number);

	FORCEINLINE int GetPlayerNumber() const { return playerNumber; }
	FORCEINLINE const FString& GetPlayerID() { return playerID; }

	const bool GetIsFalling() const;

	FORCEINLINE const float GetDirection() const { return direction; }
	FORCEINLINE const float GetPitch() const { return pitch; }
	FORCEINLINE const float GetYaw() const { return yaw; }

	FORCEINLINE const bool GetTurnRight() const { return turnRight; }
	FORCEINLINE const bool GetTurnLeft() const { return turnLeft; }

	EWeaponType GetCurrentWeaponType() const;

	FORCEINLINE CharacterInfo& GetPlayerInfo() { return myInfo; }

	void SetAttackResult(const int zombieNumber, const FHitResult& hitResult);

	bool IsWrestling();

	void PlayerDead();

	void PlayerRespawn(const bool isLocalPlayer);

	void SetHealth(const float newHealth);

	float GetHealthPercentage();

	void RecoverHealth(const float recoveryAmount);

	void EquipItem(TWeakObjectPtr<AItemBase> item, const int8 slotNumber);

public:

	void AttachItemActor(TWeakObjectPtr<AItemBase> item);

	void UnEquipItem(TWeakObjectPtr<AItemBase> item);

	void ShowHealthWidget();

	void InitializeHealthWidget();

	void SetMaxHealth(const int newHealth);

	UFUNCTION(BlueprintCallable)
	void AttachDisarmedWeaponToBack();

	void ChangeWeapon(TWeakObjectPtr<AItemBase> changedWeaponActor);

	bool DisarmWeapon();

	TWeakObjectPtr<AItemBase> GetArmedWeapon() const;

	bool TryActivateWeaponAbility(const EInputType inputType);

	bool TryActivateInputAbility(const EInputType inputType);

	void ActivateInputInterval();
	void DeactivateInputInterval();
	bool IsInputInterval() const { return bInputInterval; };

	void ActivateAiming();
	void DeactivateAiming();

	UFUNCTION(BlueprintCallable)
	bool GetAiming() const { return bAiming; };

	void SetTargetSpeed(const float speed);

	FORCEINLINE float GetTargetSpeed() const { return targetSpeed; };

protected:

	void HideHealthWidget();

private:

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> asc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* followCamera;

	UPROPERTY()
	USphereComponent* playerRange;

	UPROPERTY()
	UPlayerAnimInst* animInst;

	UPROPERTY()
	USkeletalMeshComponent* HeadMeshComponent;

	UPROPERTY()
	USkeletalMeshComponent* TopMeshComponent;

	UPROPERTY()
	USkeletalMeshComponent* BottomMeshComponent;

	UPROPERTY()
	USkeletalMeshComponent* FootMeshComponent;

	UPROPERTY(EditAnywhere, Category = "GAS | Abilities", meta = (AllowPrivateAccess = "true"))
	TMap<EInputType, TSubclassOf<UGameplayAbility>> inputAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS | Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> defaultAbilities;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* defaultMappingContext;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* jumpAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* moveAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* lookAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* healthWidget;

	UPROPERTY(EditAnywhere, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundCue* deathSound;

	TWeakObjectPtr<UWidgetPlayerHealth> healthWidgetObject;

	int playerNumber;

	FString playerID;

	FVector velocity;

	float direction, pitch, yaw;

	bool turnRight, turnLeft;

	CharacterInfo myInfo;

	float health;

	float maxHealth;

	EWeaponType currentWeaponType = EWeaponType::NONE;

	FTimerHandle healthWidgetDeacitvateTimer;

	TWeakObjectPtr<AItemBase> armedWeapon;

	bool bInputInterval = false;

	UPROPERTY(VisibleAnywhere, Category = "Status")
	bool bAiming = false;

	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float targetSpeed = 0.f;

	bool isLocal = false;

};

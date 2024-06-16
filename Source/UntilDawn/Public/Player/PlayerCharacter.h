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

protected:

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* newController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

protected:

	void Jump();

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

public:

	DelegateZombieInRange DZombieInRange;
	DelegateZombieInRange DZombieOutRange;

protected:

	UFUNCTION()
	void OnPlayerRangeComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerRangeComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	virtual void Tick(float deltaTime) override;

	void SetTargetSpeed(const float speed);

protected:

	void SetPitchAndYaw(float deltaTime);

public:

	/* 게임플레이 어빌리티 함수 */

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	bool TryActivateWeaponAbility(const EInputType inputType);

	bool TryActivateInputAbility(const EInputType inputType);

	/* 아이템 장착 / 장착 해제 */

	void EquipItem(TWeakObjectPtr<AItemBase> item, const int8 slotNumber);

protected:

	void AttachItemActor(TWeakObjectPtr<AItemBase> item);

public:

	bool ArmWeapon(TWeakObjectPtr<AItemBase> itemActor);

	void UnEquipItem(TWeakObjectPtr<AItemBase> item);

	bool DisarmWeapon();

	void ChangeWeapon(TWeakObjectPtr<AItemBase> changedWeaponActor);

protected:

	UFUNCTION(BlueprintCallable)
	void AttachDisarmedWeaponToBack();

public:	

	FORCEINLINE EWeaponType GetCurrentWeaponType() const { return currentWeaponType; }

	FORCEINLINE TWeakObjectPtr<AItemBase> GetArmedWeapon() const { return armedWeapon; }

	/* 동기화 */

	void UpdatePlayerInfo();

	/* 체력 */

	void InitializeHealthWidget();

	void ShowHealthWidget();

	void RecoverHealth(const float recoveryAmount);

protected:

	void HideHealthWidget();

public:

	void SetHealth(const float newHealth);

	void SetMaxHealth(const int newHealth);

	float GetHealthPercentage();

	/* 죽음, 리스폰 */

	void PlayerDead();

	void PlayerRespawn();

	/* 플레이어 정보 */

	void SetPlayerIDAndNumber(const FString& id, const int number);

	FORCEINLINE int GetPlayerNumber() const { return playerNumber; }

	FORCEINLINE const FString& GetPlayerID() { return playerID; }

	/* 움직임 상태 */

	const bool GetIsFalling() const;

	FORCEINLINE const float GetDirection() const { return direction; }
	FORCEINLINE const float GetPitch() const { return pitch; }
	FORCEINLINE const float GetYaw() const { return yaw; }

	FORCEINLINE const bool GetTurnRight() const { return turnRight; }
	FORCEINLINE const bool GetTurnLeft() const { return turnLeft; }

	FORCEINLINE float GetTargetSpeed() const { return targetSpeed; };

	void SetPitch(const float newPitch);

	/* 플레이어 상태 */

	FORCEINLINE CharacterInfo& GetPlayerInfo() { return myInfo; }

	bool IsWrestling();

	void ActivateInputInterval();
	void DeactivateInputInterval();

	bool IsInputInterval() const { return bInputInterval; };

	void ActivateAiming();
	void DeactivateAiming();

	UFUNCTION(BlueprintCallable)
	bool GetAiming() const { return bAiming; };

private:

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> asc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* cameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* followCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float targetSpeed = 0.f;

	CharacterInfo myInfo;

	float health;

	float maxHealth;

	FTimerHandle healthWidgetDeacitvateTimer;

	EWeaponType currentWeaponType = EWeaponType::NONE;

	TWeakObjectPtr<AItemBase> armedWeapon;

	bool bInputInterval = false;

	UPROPERTY(VisibleAnywhere, Category = "Status")
	bool bAiming = false;

	bool isLocal = false;

};

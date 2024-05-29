// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "../Enums/WeaponType.h"
#include "../Enums/ArmourSlot.h"
#include "Structs/CharacterInfo.h"
#include "Structs/Tile.h"
#include "PlayerCharacter.generated.h"

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

DECLARE_DELEGATE_OneParam(DelegateZombieInRange, int zombieNumber);
DECLARE_DELEGATE_OneParam(DelegateZombieOutRange, int zombieNumber);
DECLARE_DELEGATE_TwoParams(DelegateZombieHitsMe, int zombieNumber, bool bResult);

enum class EBowStatus : uint8
{
	Loaded,
	Drawed,
	Full = 3,
};


UCLASS()
class UNTILDAWN_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	DelegateZombieInRange DZombieInRange;
	DelegateZombieInRange DZombieOutRange;
	DelegateZombieHitsMe DZombieHitsMe;

protected:

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* newController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

	void Jump();

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void Sprint();

	void SprintEnd();

public:

	bool CheckAbleInput();

	bool LeftClick(const EWeaponType weaponType);

	bool LeftClickHold(const EWeaponType weaponType);

	bool LeftClickEnd(const EWeaponType weaponType);

	bool RightClick(const EWeaponType weaponType);

	bool RightClickEnd(const EWeaponType weaponType);

	bool ArmWeapon(TWeakObjectPtr<AItemBase> itemActor);

	bool HKeyPressed();

	void SuccessToBlocking();

protected:

	UFUNCTION()
	void OnPlayerRangeComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerRangeComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	virtual void Tick(float deltaTime) override;

	void UpdatePlayerInfo();

	void SetPlayerIDAndNumber(const FString& id, const int number);

	FORCEINLINE int GetPlayerNumber() const { return playerNumber; }
	FORCEINLINE const FString& GetPlayerID() { return playerID; }

	const bool GetIsFalling() const;

	FORCEINLINE const float GetSpeed() const { return speed; }

	FORCEINLINE const float GetDirection() const { return direction; }
	FORCEINLINE const float GetPitch() const { return pitch; }

	FORCEINLINE const bool GetTurnRight() const { return turnRight; }
	FORCEINLINE const bool GetTurnLeft() const { return turnLeft; }
	FORCEINLINE const bool GetRightClick() const { return rightClick; }

	EWeaponType GetCurrentWeaponType() const;

	void SetCurrentWeaponType(const EWeaponType weaponType);

	FORCEINLINE CharacterInfo& GetPlayerInfo() { return myInfo; }

	void DoPlayerInputAction(const int inputType, const int weaponType);

	void SetAttackResult(const bool result, const int zombieNumber);

	FORCEINLINE void SetWrestlingOn();
	FORCEINLINE void SetWrestlingOff();
	FORCEINLINE bool GetWrestling() { return bWrestling; }
	FORCEINLINE uint8 GetBowStatus() const { return bowStatus; }

	void PlayPushingZombieMontage(const bool isBlocking);

	UFUNCTION()
	void FailedToResist();

	UFUNCTION()
	void WrestlingEnd();

	void PlayerDead();

	void InitializePlayerInfo();

	void PlayerRespawn(const bool isLocalPlayer);

	void DeadReckoningMovement(const FVector& lastLocation, const FVector& lastVelocity, const double ratency);

	void SetHealth(const float newHealth);

	float GetHealthPercentage();

	void RecoverHealth(const float recoveryAmount);

	void EquipItem(TWeakObjectPtr<AItemBase> item, const int8 slotNumber);

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

protected:

	void HideHealthWidget();

private:

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

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* defaultMappingContext;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* jumpAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* moveAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* lookAction;

	UPROPERTY(VisibleDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* sprintAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* healthWidget;

	TWeakObjectPtr<UWidgetPlayerHealth> healthWidgetObject;

	int playerNumber;

	FString playerID;

	FVector velocity;

	float speed, direction, pitch;

	bool turnRight, turnLeft, rightClick;

	uint8 bowStatus;

	float shootPower;

	CharacterInfo myInfo;

	bool bWrestling;

	FVector nextLocation;

	bool bset;

	float health;

	float maxHealth;

	EWeaponType currentWeaponType = EWeaponType::NONE;

	FTimerHandle healthWidgetDeacitvateTimer;

	TWeakObjectPtr<AItemBase> armedWeapon;

};

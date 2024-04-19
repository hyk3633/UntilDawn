// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Enums/WeaponType.h"
#include "Structs/CharacterInfo.h"
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
class UInventoryComponent;

DECLARE_DELEGATE_OneParam(DelegateZombieInRange, int zombieNumber);
DECLARE_DELEGATE_OneParam(DelegateZombieOutRange, int zombieNumber);
DECLARE_DELEGATE_TwoParams(DelegateZombieHitsMe, int zombieNumber, bool bResult);
DECLARE_DELEGATE_OneParam(DelegateHitZombie, int zombieNumber);
DECLARE_DELEGATE_OneParam(DelegateHitPlayer, int playerNumber);

UCLASS()
class UNTILDAWN_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	DelegateZombieInRange DZombieInRange;
	DelegateZombieInRange DZombieOutRange;
	DelegateZombieHitsMe DZombieHitsMe;
	DelegateHitZombie DHitZombie;
	DelegateHitPlayer DHitPlayer;

protected:

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* newController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

	bool CheckAbleInput();

	void Jump();

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void Sprint();

	void SprintEnd();

public:

	bool LeftClick();

	bool LeftClickHold();

	bool LeftClickEnd();

	bool RightClick();

	bool RightClickEnd();

	bool RKeyPressed();

	bool RKeyHold();

	void SuccessToBlocking();

protected:

	UFUNCTION()
	void OnPlayerRangeComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerRangeComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	virtual void Tick(float deltaTime) override;

	void UpdatePlayerInfo();

	FORCEINLINE void SetPlayerNumber(const int num) { number = num; }
	FORCEINLINE int GetPlayerNumber() const { return number; }

	FORCEINLINE void SetPlayerID(const FString& id) { playerID = id; }

	FORCEINLINE const FString& GetPlayerID() { return playerID; }

	const bool GetIsFalling() const;

	FORCEINLINE const float GetSpeed() const { return speed; }

	FORCEINLINE const float GetDirection() const { return direction; }
	FORCEINLINE const float GetPitch() const { return pitch; }

	FORCEINLINE const bool GetTurnRight() const { return turnRight; }
	FORCEINLINE const bool GetTurnLeft() const { return turnLeft; }
	FORCEINLINE const bool GetRightClick() const { return rightClick; }
	FORCEINLINE const bool GetIsAbleShoot() const { return isAbleShoot; }
	FORCEINLINE EWeaponType GetCurrentWeaponType() const { return currentWeaponType; }

	FORCEINLINE PlayerInfo& GetPlayerInfo() { return myInfo; }

	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return inventoryComponent; }

	void DoPlayerInputAction(const int inputType);

	void SetAttackResult(const bool result, const int zombieNumber);

	FORCEINLINE void SetWrestlingOn();
	FORCEINLINE void SetWrestlingOff();
	FORCEINLINE bool GetWrestling() { return bWrestling; }

	void PlayPushingZombieMontage(const bool isBlocking);

	UFUNCTION()
	void FailedToResist();

	UFUNCTION()
	void WrestlingEnd();

	void StartAttack();

	void ActivateAttackTrace();

	void EndAttack();

	FORCEINLINE bool GetAttackActivated() const { return isAttackActivated; }

	bool AddItemToInventory(TWeakObjectPtr<UItemObject> itemObj);

	void PlayerDead();

	void InitializePlayerInfo();

	void PlayerRespawn(const bool isLocalPlayer);

	void DeadReckoningMovement(const FVector& lastLocation, const FVector& lastVelocity, const double ratency);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* followCamera;

	UPROPERTY()
	UInventoryComponent* inventoryComponent;

	UPROPERTY()
	USphereComponent* playerRange;

	UPROPERTY()
	UPlayerAnimInst* animInst;

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

	int number;

	FString playerID;

	FVector velocity;

	float speed, direction, pitch;

	bool turnRight, turnLeft, rightClick;

	bool isAbleShoot;

	float shootPower;

	EWeaponType currentWeaponType = EWeaponType::NONE;

	PlayerInfo myInfo;

	bool bWrestling;

	bool isAttackActivated;

	TWeakObjectPtr<AItemMeleeWeapon> equippedWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<AItemBase*> items;

	FVector nextLocation;

	bool bset;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/ZombieState.h"
#include "Structs/Pos.h"
#include "Structs/CharacterInfo.h"
#include "../Interface/PoolableActor.h"
#include "AbilitySystemInterface.h"
#include "Engine/StreamableManager.h"
#include "ZombieCharacter.generated.h"

class UZombieAnimInstance;
class APlayerCharacter;
class UWidgetComponent;
class UWidgetZombieHealth;
class UGameplayAbility;
class USoundCue;
class UParticleSystem;

DECLARE_DELEGATE_OneParam(OnHealthChanged, float healthPercentage);

UCLASS(config = UntilDawn)
class UNTILDAWN_API AZombieCharacter : public ACharacter, public IPoolableActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AZombieCharacter();

protected:

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

public:

	OnHealthChanged onHealthChanged;

	/* 풀러블 액터 인터페이스 가상 함수 */

	virtual void ActivateActor() override;

	virtual void DeactivateActor() override;

	virtual bool IsActorActivated() override;

	/* 동기화 */

	void SetZombieInfo(const ZombieInfo& info);

protected:

	void ProcessZombieInfo(const ZombieInfo& info, const int bitType);

	void SetZombieState(const EZombieState newState);

	void SetNextLocation(const FVector& nextLoc);

public:

	void UpdateHealth(const float newHealth);

protected:

	void HideHealthWidget();

	/* 이동 */

	void UpdateMovement();

	void FollowPath();

	void StartMovementUpdate();

public:

	/* 공격 */

	void StartAttack();

	void EndAttack();

	void AttackFailed();

	void ActivateAttackTrace(const int attackAnimationType);

protected:

	void PlayHitEffect(const FHitResult& hit);

public:

	/* 죽음 */

	void ZombieDead();

protected:

	void DeactivateAfterDelay();

public:

	/* Getter Setter */

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE void SetNumber(const int newNumber) { number = newNumber; }

	FORCEINLINE const int GetNumber() const { return number; }

	FORCEINLINE void SetTarget(APlayerCharacter* target) { targetPlayer = target; }

	UFUNCTION(BlueprintCallable)
	EZombieState GetZombieState() const;

	UFUNCTION(BlueprintCallable)
	float GetSpeed() const;

	void SetSkeletalMesh(USkeletalMesh* skeletalMesh);

	FORCEINLINE int GetAttackActivated() const { return isAttackActivated; }

	FName GetSocketName(const int animationType);

private:

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> asc;

	UPROPERTY()
	TSubclassOf<UGameplayAbility> hitReactionAbility;

	UPROPERTY()
	TSubclassOf<UGameplayAbility> kickReactionAbility;

	UPROPERTY()
	UZombieAnimInstance* animInst;

	UPROPERTY()
	UWidgetComponent* healthWidget;

	TWeakObjectPtr<UWidgetZombieHealth> healthWidgetObject;

	UPROPERTY(EditAnywhere, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* impactParticle;

	UPROPERTY(EditAnywhere, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundCue* deathSound;

	UPROPERTY(EditAnywhere, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundCue* impactSound;

	int number;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	bool isActive;

	bool isAttackActivated;

	int16 attackPower = 30;

	float health = 200;

	float maxHealth = 200;

	float speed = 100.f;

	float interval = 0.016f;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	EZombieState state;

	UPROPERTY()
	APlayerCharacter* targetPlayer;

	FVector nextPoint, nextDirection;

	FTimerHandle movementUpdateTimer;

	FTimerHandle deactivateDelayTimer;

	FTimerHandle healthWidgetHideTimer;

};

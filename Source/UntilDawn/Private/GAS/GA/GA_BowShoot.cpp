// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_BowShoot.h"
#include "Player/PlayerCharacter.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject.h"
#include "Item/ItemObject/ItemProjectileWeapon.h"
#include "Item/ItemObject/ItemAmmo.h"
#include "Item/Projectile/ProjectileBase.h"
#include "GameSystem/InventoryComponent.h"
#include "GameMode/GameModeMainMap.h"
#include "GAS/AttributeSet/PlayerAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "Tag/UntilDawnGameplayTags.h"
#include "Engine/SkeletalMeshSocket.h"

UGA_BowShoot::UGA_BowShoot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bRetriggerInstancedAbility = true;

	static ConstructorHelpers::FClassFinder<UAnimInstance> bowAnimInstRef(TEXT("/Script/Engine.AnimBlueprint'/Game/_Assets/Animations/Bow/AnimBP_Bow.AnimBP_Bow_C'"));
	if (bowAnimInstRef.Succeeded()) bowAnimInstClass = bowAnimInstRef.Class;
}

void UGA_BowShoot::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	character->DeactivateAiming();
	ProcessReleased();
}

void UGA_BowShoot::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ProcessPressed();
}

void UGA_BowShoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	controller = Cast<APlayerControllerMainMap>(character->GetController());
	weaponObject = Cast<UItemPermanent>(character->GetArmedWeapon()->GetItemObject());

	if (controller.IsValid())
	{
		if (IsAbleShoot())
		{
			SendAbilityActivationToController();
		}
		else
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}
	}

	bowMesh = character->GetArmedWeapon()->GetSkeletalMesh();
	if (bowAnimInstClass)
	{
		bowMesh->SetAnimClass(bowAnimInstClass);
	}

	if (arrowClass)
	{
		arrow = GetWorld()->SpawnActor<AActor>(arrowClass);
		arrow->SetActorHiddenInGame(false);
		const USkeletalMeshSocket* socket = bowMesh->GetSocketByName("ArrowSocket");
		check(socket);
		socket->AttachActor(arrow, bowMesh);
	}

	bowState = EBowState::IdleToAim;

	if (bowShootMontage && bowMeshMontage)
	{
		CreateAbilityTask();
		GetWorld()->GetTimerManager().SetTimer(holdTimer, this, &UGA_BowShoot::SetAbleShoot, holdTime);
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_BowShoot::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UGA_BowShoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (arrow)
	{
		arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		arrow->SetActorHiddenInGame(true);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_BowShoot::CreateAbilityTask()
{
	const FName sectionName = *FString::FromInt(StaticCast<uint8>(bowState));

	UAbilityTask_PlayMontageAndWait* playBowShootTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), bowShootMontage, 1.f, sectionName);
	playBowShootTask->OnCompleted.AddDynamic(this, &UGA_BowShoot::OnCompleteCallback);
	playBowShootTask->OnInterrupted.AddDynamic(this, &UGA_BowShoot::OnInterruptedCallback);
	playBowShootTask->SetWaitingOnAvatar();
	playBowShootTask->ReadyForActivation();

	bowMesh->GetAnimInstance()->Montage_Play(bowMeshMontage);
	bowMesh->GetAnimInstance()->Montage_JumpToSection(sectionName);
	
	bMontageEnded = false;
}

bool UGA_BowShoot::IsAbleShoot()
{
	TWeakObjectPtr<UItemProjectileWeapon> projectileWeaponObject = Cast<UItemProjectileWeapon>(weaponObject);
	if (projectileWeaponObject->GetLoadedAmmoAmount())
	{
		return true;
	}
	else
	{
		TWeakObjectPtr<UInventoryComponent> inventoryComponent = controller->GetInventoryComponent();
		auto ammoItem = inventoryComponent->FindAmmo(projectileWeaponObject->GetAmmoType());
		if (ammoItem.IsValid())
		{
			return inventoryComponent->FindAmmo(ammoItem->GetAmmoType()).IsValid();
		}
		else
		{
			return false;
		}
	}
}

void UGA_BowShoot::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_BowShoot::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_BowShoot::ProcessReleased()
{
	switch (bowState)
	{
		case EBowState::IdleToAim:
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			break;
		}
		case EBowState::Aiming:
		{
			AbilityTags.RemoveTag(UD_CHARACTER_STATE_AIMING);
			arrow->SetActorHiddenInGame(true);
			if (IsStaminaEnough())
			{
				SendAbilityActivationToController();
				bowState = EBowState::ShootToIdle;
				PlayMontage();
				ApplyGameplayEffect();
			}
			else
			{
				MontageJumpToSection(*FString::FromInt(4));
			}
			break;
		}
		case EBowState::ShootToIdle:
		{
			break;
		}
		case EBowState::ShootToAim:
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			break;
		}
		case EBowState::AimToIdle:
		{
			break;
		}
	}
}

void UGA_BowShoot::ProcessPressed()
{
	switch (bowState)
	{
		case EBowState::IdleToAim:
		{
			SendAbilityActivationToController();
			bowState = EBowState::Aiming;
			AbilityTags.AddTag(UD_CHARACTER_STATE_AIMING);
			break;
		}
		case EBowState::Aiming:
		{
			break;
		}
		case EBowState::ShootToIdle:
		{
			if (character->IsInputInterval())
			{
				SendAbilityActivationToController();
				bowState = EBowState::ShootToAim;
				arrow->SetActorHiddenInGame(false);
				PlayMontage();
				GetWorld()->GetTimerManager().SetTimer(holdTimer, this, &UGA_BowShoot::SetAbleShoot, holdTime);
			}
			break;
		}
		case EBowState::ShootToAim:
		{
			SendAbilityActivationToController();
			bowState = EBowState::Aiming;
			arrow->SetActorHiddenInGame(false);
			AbilityTags.AddTag(UD_CHARACTER_STATE_AIMING);
			PlayMontage();
			break;
		}
		case EBowState::AimToIdle:
		{
			SendAbilityActivationToController();
			bowState = EBowState::IdleToAim;
			arrow->SetActorHiddenInGame(true);
			PlayMontage();
			GetWorld()->GetTimerManager().SetTimer(holdTimer, this, &UGA_BowShoot::SetAbleShoot, holdTime);
			break;
		}
	}
}

void UGA_BowShoot::SetAbleShoot()
{
	bowState = EBowState::Aiming;
	character->ActivateAiming();
	AbilityTags.AddTag(UD_CHARACTER_STATE_AIMING);
}

void UGA_BowShoot::PlayMontage()
{
	const FName sectionName = *FString::FromInt(StaticCast<uint8>(bowState));
	MontageJumpToSection(sectionName);
	bowMesh->GetAnimInstance()->Montage_Play(bowMeshMontage);
	bowMesh->GetAnimInstance()->Montage_JumpToSection(sectionName);
}

void UGA_BowShoot::SendAbilityActivationToController()
{
	if (controller.IsValid())
	{
		FGameplayAbilitySpec* specPtr = character->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(CurrentSpecHandle);
		controller->SendActivatedWeaponAbility(specPtr->InputID);
	}
}

bool UGA_BowShoot::IsStaminaEnough()
{
	if (controller.IsValid() == false)
		return true;

	const float currentStamina = character->GetAbilitySystemComponent()->GetNumericAttributeBase(UPlayerAttributeSet::GetStaminaAttribute());
	if (currentStamina > GetCostGameplayEffect()->Modifiers[0].Magnitude.GetValue())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UGA_BowShoot::ApplyGameplayEffect()
{
	if (controller.IsValid() == false)
		return;

	FGameplayEffectSpecHandle effectSpecHandle = MakeOutgoingGameplayEffectSpec(GetCostGameplayEffect()->GetClass());
	if (effectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, effectSpecHandle);
	}
}


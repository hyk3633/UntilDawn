// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFunction/FunctionMeleeAttack.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UntilDawn/UntilDawn.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/UntilDawnGameplayTags.h"

FunctionMeleeAttack::FunctionMeleeAttack()
{
}

FunctionMeleeAttack::~FunctionMeleeAttack()
{
}

void FunctionMeleeAttack::MeleeAttack(TWeakObjectPtr<APlayerControllerMainMap> attackerController, USkeletalMeshComponent* weaponMesh, const FString& itemID)
{
	check(attackerController.IsValid());

	FVector collisionLocation = weaponMesh->GetSocketLocation(FName("CollisionSocket"));
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(attackerController->GetPawn());
	FHitResult hit;
	UKismetSystemLibrary::SphereTraceSingle
	(
		attackerController.Get(),
		collisionLocation,
		collisionLocation + FVector(0, 0, 1),
		24,
		UEngineTypes::ConvertToTraceType(ECC_PlayerAttack),
		false,
		actorsToIgnore,
		EDrawDebugTrace::None,
		hit,
		true
	);

	if (hit.bBlockingHit)
	{
		FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(hit);
		FGameplayEventData payloadData;
		payloadData.TargetData.Add(targetData);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(hit.GetActor(), UD_EVENT_CHARACTER_HITREACTION, payloadData);

		attackerController->EndAttack();
		attackerController->SendHittedCharacter(hit, itemID);
	}
}

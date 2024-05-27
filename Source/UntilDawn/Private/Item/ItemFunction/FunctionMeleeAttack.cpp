// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFunction/FunctionMeleeAttack.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UntilDawn/UntilDawn.h"

FunctionMeleeAttack::FunctionMeleeAttack()
{
}

FunctionMeleeAttack::~FunctionMeleeAttack()
{
}

void FunctionMeleeAttack::MeleeAttack(TWeakObjectPtr<APlayerControllerMainMap> attackerController, USkeletalMeshComponent* weaponMesh, const float stkPower)
{
	check(attackerController.IsValid());

	FHitResult hit;
	FVector collisionLocation = weaponMesh->GetSocketLocation(FName("CollisionSocket"));
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(attackerController->GetPawn());
	TArray<FHitResult> hits;
	UKismetSystemLibrary::SphereTraceMulti
	(
		attackerController.Get(),
		collisionLocation,
		collisionLocation + FVector(0, 0, 1),
		12,
		UEngineTypes::ConvertToTraceType(ECC_PlayerAttack),
		false,
		actorsToIgnore,
		EDrawDebugTrace::Persistent,
		hits,
		true
	);
	
	if (hits.Num())
	{
		attackerController->SendHittedCharacters(hits, stkPower);
	}
}

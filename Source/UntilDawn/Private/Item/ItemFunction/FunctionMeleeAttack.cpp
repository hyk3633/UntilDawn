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

void FunctionMeleeAttack::MeleeAttack(TWeakObjectPtr<APlayerController> attackerController, USkeletalMeshComponent* weaponMesh)
{
	FHitResult hit;
	FVector collisionLocation = weaponMesh->GetSocketLocation(FName("CollisionSocket"));
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(attackerController->GetPawn());
	TArray<FHitResult> hits;
	UKismetSystemLibrary::SphereTraceMulti
	(
		attackerController.Get(),
		collisionLocation,
		collisionLocation,
		12,
		UEngineTypes::ConvertToTraceType(ECC_PlayerAttack),
		false,
		actorsToIgnore,
		EDrawDebugTrace::Persistent,
		hits,
		true
	);

	TWeakObjectPtr<APlayerControllerMainMap> mainMapController = Cast<APlayerControllerMainMap>(attackerController);
	if (mainMapController.IsValid() && hits.Num())
	{
		mainMapController->SendHittedCharacters(hits);
	}
}

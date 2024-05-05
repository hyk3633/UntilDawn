// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Component/MeleeAttackComponent.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UntilDawn/UntilDawn.h"

UMeleeAttackComponent::UMeleeAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UMeleeAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMeleeAttackComponent::Smash(TWeakObjectPtr<APlayerController> attackerController, USkeletalMeshComponent* weaponMesh)
{
	FHitResult hit;
	FVector collisionLocation = weaponMesh->GetSocketLocation(FName("CollisionSocket"));
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(attackerController->GetPawn());
	TArray<FHitResult> hits;
	UKismetSystemLibrary::SphereTraceMulti
	(
		this,
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
	
}


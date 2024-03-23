// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/ItemMeleeWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UntilDawn/UntilDawn.h"

AItemMeleeWeapon::AItemMeleeWeapon()
{

}

void AItemMeleeWeapon::ActivateAttackTrace(FHitResult& hit)
{
	const FVector socketLocation = GetSkeletalMesh()->GetSocketLocation(FName("CollisionSocket"));
	UKismetSystemLibrary::SphereTraceSingle
	(
		this,
		socketLocation,
		socketLocation,
		16,
		UEngineTypes::ConvertToTraceType(ECC_PlayerAttack),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForDuration,
		hit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.f
	);
}

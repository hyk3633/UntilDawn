// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/ItemMeleeWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UntilDawn/UntilDawn.h"

AItemMeleeWeapon::AItemMeleeWeapon()
{

}

void AItemMeleeWeapon::InitializeWeaponInfo(const FWeaponInfo& newInfo)
{
	weaponInfo = newInfo;
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

void AItemMeleeWeapon::Attack(TWeakObjectPtr<APlayerController> ownerController)
{
}

EWeaponType AItemMeleeWeapon::GetWeaponType() const
{
	return static_cast<EWeaponType>(weaponInfo.weaponType);
}

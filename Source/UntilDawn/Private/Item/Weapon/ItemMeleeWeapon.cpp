// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/ItemMeleeWeapon.h"
#include "Item/Component/MeleeAttackComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UntilDawn/UntilDawn.h"

AItemMeleeWeapon::AItemMeleeWeapon()
{
	meleeAttackComponent = CreateDefaultSubobject<UMeleeAttackComponent>(TEXT("Melee Attack Component"));
}

void AItemMeleeWeapon::InitializeWeaponInfo(const FWeaponInfo& newInfo)
{
	weaponInfo = newInfo;
}

void AItemMeleeWeapon::Attack(TWeakObjectPtr<APlayerController> ownerController)
{
	meleeAttackComponent->Smash(ownerController, GetSkeletalMesh());
}

EWeaponType AItemMeleeWeapon::GetWeaponType() const
{
	return static_cast<EWeaponType>(weaponInfo.weaponType);
}

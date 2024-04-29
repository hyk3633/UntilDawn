// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/ItemWeapon.h"

void AItemWeapon::Attack(TWeakObjectPtr<APlayerController> ownerController)
{
}

EWeaponType AItemWeapon::GetWeaponType() const
{
    return EWeaponType::NONE;
}

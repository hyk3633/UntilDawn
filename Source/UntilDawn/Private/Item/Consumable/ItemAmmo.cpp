// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumable/ItemAmmo.h"

void AItemAmmo::InitializeAmmoInfo(const FAmmoItemInfo& newInfo)
{
	ammoItemInfo = newInfo;
}

void AItemAmmo::UsingItem()
{
	ammoItemInfo.amount = FMath::Max(ammoItemInfo.amount - 1, 0);
	if (ammoItemInfo.amount == 0)
	{
		// 아이템 만료 델리게이트 호출
	}
}

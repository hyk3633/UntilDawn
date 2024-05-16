// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemAmmo.h"

void UItemAmmo::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemAmmo::MakeItemFunction()
{
}

uint16 UItemAmmo::Using(const uint16 neededAmount)
{
	const uint16 remainedAmmo = GetItemInfo().quantity;
	Consumed(neededAmount);
	if (remainedAmmo >= neededAmount)
	{
		return neededAmount;
	}
	else
	{
		return remainedAmmo;
	}
}

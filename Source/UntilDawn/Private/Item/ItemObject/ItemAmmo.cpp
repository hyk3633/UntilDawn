// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemAmmo.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"

void UItemAmmo::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemAmmo::MakeItemFunction()
{
}

void UItemAmmo::Using(const uint16 neededAmount)
{
	Consumed(neededAmount);
}

EAmmoType UItemAmmo::GetAmmoType() const
{
	return concreteInfo.ammoType;
}

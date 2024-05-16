// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemRecovery.h"

void UItemRecovery::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemRecovery::MakeItemFunction()
{
}

uint16 UItemRecovery::Using(const uint16 neededAmount)
{
	return uint16();
}

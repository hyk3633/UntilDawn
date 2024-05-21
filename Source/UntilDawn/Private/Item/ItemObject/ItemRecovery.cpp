// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemRecovery.h"
#include "Player/Main/PlayerControllerMainMap.h"
#include "Player/PlayerCharacter.h"

void UItemRecovery::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemRecovery::MakeItemFunction()
{
}

void UItemRecovery::Using(const uint16 neededAmount)
{
	Consumed(neededAmount);
	// animation
	// particle
	UE_LOG(LogTemp, Warning, TEXT("%d recovery use"), neededAmount);
}

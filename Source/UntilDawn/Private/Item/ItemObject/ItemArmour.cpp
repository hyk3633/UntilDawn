// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemArmour.h"

void UItemArmour::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemArmour::MakeItemFunction()
{

}

uint8 UItemArmour::GetItemSubType() const
{
	return StaticCast<uint8>(concreteInfo.armourSlot);
}

void UItemArmour::Using(USkeletalMeshComponent* itemMesh)
{

}

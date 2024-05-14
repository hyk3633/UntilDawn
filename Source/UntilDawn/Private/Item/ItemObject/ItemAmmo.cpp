// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemAmmo.h"

void UItemAmmo::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemAmmo::MakeItemFunction()
{
}

void UItemAmmo::Using(TWeakObjectPtr<APlayerController> playerController, USkeletalMeshComponent* itemMesh)
{
}

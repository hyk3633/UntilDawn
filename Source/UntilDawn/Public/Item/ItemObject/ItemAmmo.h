// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject.h"
#include "../../Structs/ItemInfo.h"
#include "ItemAmmo.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UItemAmmo : public UItemObject
{
	GENERATED_BODY()

public:

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

public:

	virtual void Using(TWeakObjectPtr<APlayerController> playerController, USkeletalMeshComponent* itemMesh = nullptr) override;

private:

	FAmmoItemInfo concreteInfo;
	
};

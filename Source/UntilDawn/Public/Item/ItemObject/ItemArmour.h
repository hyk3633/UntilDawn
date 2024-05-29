// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject/ItemPermanent.h"
#include "../../Structs/ItemInfo.h"
#include "ItemArmour.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UItemArmour : public UItemPermanent
{
	GENERATED_BODY()
	
public:

	UItemArmour() = default;
	~UItemArmour() = default;

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	virtual void MakeItemFunction() override;

public:

	virtual uint8 GetItemSubType() const override;

	virtual void Using(USkeletalMeshComponent* itemMesh = nullptr) override;

private:

	FArmourItemInfo concreteInfo;

};

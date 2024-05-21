// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject.h"
#include "../../Enums/PermanentItemType.h"
#include "ItemPermanent.generated.h"

/**
 * 
 */
UCLASS()
class UNTILDAWN_API UItemPermanent : public UItemObject
{
	GENERATED_BODY()

public:

	virtual void Using(USkeletalMeshComponent* itemMesh = nullptr) PURE_VIRTUAL(UItemPermanant::Using, );
	
	EPermanentItemType GetPermanentItemType() const;

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) override;

	void SetPermanentItemType(const int type);

private:

	EPermanentItemType permanentItemType;

};

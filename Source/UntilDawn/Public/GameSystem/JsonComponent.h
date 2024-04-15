// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/ItemType.h"
#include "Structs/ItemInfo.h"
#include "Structs/ItemAsset.h"
#include "JsonComponent.generated.h"

class ItemCore;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UJsonComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UJsonComponent();

	virtual void BeginDestroy() override;

protected:

	virtual void BeginPlay() override;

	void Initialize();

	void ReadJson(const TArray<TSharedPtr<FJsonValue>>* jsonItems);

public:

	void FillItemInfoMap(TMap<int, FItemInfo*>& infoMap);

	void FillItemAssetMap(TMap<int, TSharedPtr<FItemAsset>>& assetMap);

	void GetData(const int itemKey, FItemInfo* newInfo);

protected:


private:	
	
	// 아이템 타입 별로 tmap을 만들어서 각 타입에 맞는 tmap에 저장 (키는 itemKey, 밸류는 아이템 구조체)
	TMap<int, FItemInfo*> itemInfoMap;

	UPROPERTY(VisibleAnywhere, Category = "Item Info", meta = (AllowPrivateAccess = "true"))
	UDataTable* itemAssetDataTable;

};

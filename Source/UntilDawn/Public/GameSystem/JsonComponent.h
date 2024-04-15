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
	
	// ������ Ÿ�� ���� tmap�� ���� �� Ÿ�Կ� �´� tmap�� ���� (Ű�� itemKey, ����� ������ ����ü)
	TMap<int, FItemInfo*> itemInfoMap;

	UPROPERTY(VisibleAnywhere, Category = "Item Info", meta = (AllowPrivateAccess = "true"))
	UDataTable* itemAssetDataTable;

};

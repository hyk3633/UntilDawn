// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Enums/ItemType.h"
#include "Structs/ItemInfo.h"
#include "Structs/ItemAsset.h"
#include "CoreMinimal.h"

class ItemCore;

class UNTILDAWN_API JsonComponent
{
public:	

	JsonComponent();
	~JsonComponent();

	void Initialize();

protected:

	void ReadJson(const TArray<TSharedPtr<FJsonValue>>* jsonItems);

public:

	void FillItemInfoMap(TMap<int, FItemInfo*>& infoMap);

	void GetData(const int itemKey, FItemInfo* newInfo);

protected:


private:	
	
	// 아이템 타입 별로 tmap을 만들어서 각 타입에 맞는 tmap에 저장 (키는 itemKey, 밸류는 아이템 구조체)
	TMap<int, FItemInfo*> itemInfoMap;

};

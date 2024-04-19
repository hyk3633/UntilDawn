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
	
	// ������ Ÿ�� ���� tmap�� ���� �� Ÿ�Կ� �´� tmap�� ���� (Ű�� itemKey, ����� ������ ����ü)
	TMap<int, FItemInfo*> itemInfoMap;

};

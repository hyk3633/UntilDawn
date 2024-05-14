// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Structs/ItemInfo.h"
#include "Serialization/BufferArchive.h"

/**
 * 
 */
class UNTILDAWN_API JsonParser
{
public:
	JsonParser();
	~JsonParser();

protected:

	void InitializeJson();

public:

	FItemInfo GetItemCommonInfo(const int itemKey);

	TMap<FString, TSharedPtr<FJsonValue>> GetItemConcreteInfoMap(const int itemKey);

protected:

	void SaveItemCommonInfo(const TSharedPtr<FJsonObject>* jsonItem, FItemInfo& itemInfo);

private:

	TSharedPtr<FJsonObject> jsonObj;

};

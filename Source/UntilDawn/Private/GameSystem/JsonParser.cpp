// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/JsonParser.h"

JsonParser::JsonParser()
{
	InitializeJson();
}

JsonParser::~JsonParser()
{
}

FItemInfo JsonParser::GetItemCommonInfo(const int itemKey)
{
	const TSharedPtr<FJsonObject>* itemJson;
	if (jsonObj->TryGetObjectField(*FString::FromInt(itemKey), itemJson))
	{
		FItemInfo itemInfo;
		SaveItemCommonInfo(itemJson, itemInfo);
		return itemInfo;
	}
	return FItemInfo();
}

TMap<FString, TSharedPtr<FJsonValue>> JsonParser::GetItemConcreteInfoMap(const int itemKey)
{
	const TSharedPtr<FJsonObject>* itemJson;
	if (jsonObj->TryGetObjectField(*FString::FromInt(itemKey), itemJson))
	{
		const TSharedPtr<FJsonObject>* concreteInfoJson;
		if (itemJson->Get()->TryGetObjectField(TEXT("ConcreteInfo"), concreteInfoJson))
		{
			return concreteInfoJson->Get()->Values;
		}
		
	}
	return TMap<FString, TSharedPtr<FJsonValue>>();
}

void JsonParser::SaveItemCommonInfo(const TSharedPtr<FJsonObject>* jsonItem, FItemInfo& itemInfo)
{
	int itemType;
	jsonItem->Get()->TryGetNumberField(TEXT("ItemKey"), itemInfo.itemKey);
	jsonItem->Get()->TryGetStringField(TEXT("ItemName"), itemInfo.itemName);
	jsonItem->Get()->TryGetNumberField(TEXT("ItemType"), itemType);
	jsonItem->Get()->TryGetNumberField(TEXT("Quantity"), itemInfo.quantity);
	jsonItem->Get()->TryGetBoolField(TEXT("IsConsumable"), itemInfo.isConsumable);
	itemInfo.itemType = static_cast<EItemMainType>(itemType);

	const TSharedPtr<FJsonObject>* itemGridSizeJsonObject;
	jsonItem->Get()->TryGetObjectField(TEXT("ItemGridSize"), itemGridSizeJsonObject);
	itemGridSizeJsonObject->Get()->TryGetNumberField(TEXT("X"), itemInfo.itemGridSize.X);
	itemGridSizeJsonObject->Get()->TryGetNumberField(TEXT("Y"), itemInfo.itemGridSize.Y);
}

void JsonParser::InitializeJson()
{
	FString fileStr;
	FString filePath = TEXT("D:\\UE5Projects\\UntilDawn\\Json\\ItemInfo.json");
	FFileHelper::LoadFileToString(fileStr, *filePath);

	const TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(fileStr);
	jsonObj = MakeShareable(new FJsonObject());

	const bool result = FJsonSerializer::Deserialize(jsonReader, jsonObj);
	check(result);
	check(jsonObj);
}
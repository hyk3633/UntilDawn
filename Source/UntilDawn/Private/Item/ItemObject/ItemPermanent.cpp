// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemPermanent.h"

void UItemPermanent::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
    SetPermanentItemType(concreteInfoMap["PermanentItemType"]->AsNumber());
}

void UItemPermanent::SetPermanentItemType(const int type)
{
    permanentItemType = StaticCast<EPermanentItemType>(type);
}

EPermanentItemType UItemPermanent::GetPermanentItemType() const
{
    return permanentItemType;
}

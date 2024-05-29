// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemPermanent.h"

void UItemPermanent::SetEquippedSlotNumber(const int8 number)
{
    equippedSlotNumber = number;
}

int8 UItemPermanent::GetEquippedSlotNumber() const
{
    return equippedSlotNumber;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemConsumable.h"
#include "Player/Main/PlayerControllerMainMap.h"

void UItemConsumable::Consumed(const uint16 consumedAmount)
{
	uint16 quantity = FMath::Max(GetItemQuantity() - consumedAmount, 0);
	SetItemQuantity(quantity);
	if (GetOwnerController().IsValid())
	{
		GetOwnerController()->SendItemUsing(GetItemID(), consumedAmount);
	}
	if (quantity == 0)
	{
		if (GetOwnerController().IsValid())
		{
			GetOwnerController()->ItemExhausted(this);
		}
		DItemExhaust.ExecuteIfBound(GetItemID());
	}
}

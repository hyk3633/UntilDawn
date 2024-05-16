// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemConsumable.h"

void UItemConsumable::Consumed(const uint16 consumedAmount)
{
	uint16 quantity = FMath::Max(GetItemQuantity() - consumedAmount, 0);
	SetItemQuantity(quantity);
	if (quantity == 0)
	{
		// delegate;
	}
}

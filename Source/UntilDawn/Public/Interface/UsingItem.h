// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UsingItem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUsingItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNTILDAWN_API IUsingItem
{
	GENERATED_BODY()

public:

	virtual void UsingItem() = 0;

};

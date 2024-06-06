// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_AmmoCheck.generated.h"

/**
 * 
 */

class UInventoryComponent;
class AItemBase;

DECLARE_DELEGATE_OneParam(AmmoCheckDelegate, bool result);

UCLASS()
class UNTILDAWN_API UAT_AmmoCheck : public UAbilityTask
{
	GENERATED_BODY()

public:

	UAT_AmmoCheck();

	static UAT_AmmoCheck* CreateTask(UGameplayAbility* owningAbility, TWeakObjectPtr<AItemBase> weaponActor, TWeakObjectPtr<UInventoryComponent> inventoryComponent);

	virtual void Activate() override;

	virtual void OnDestroy(bool ability) override;

	AmmoCheckDelegate DAmmoCheck;

protected:

	TWeakObjectPtr<UInventoryComponent> playerInventoryComponent;

	TWeakObjectPtr<AItemBase> armedWeaponActor;
	
};

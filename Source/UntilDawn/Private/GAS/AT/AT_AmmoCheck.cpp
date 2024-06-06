// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_AmmoCheck.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject.h"
#include "Item/ItemObject/ItemProjectileWeapon.h"
#include "Item/ItemObject/ItemAmmo.h"
#include "GameSystem/InventoryComponent.h"

UAT_AmmoCheck::UAT_AmmoCheck()
{
}

UAT_AmmoCheck* UAT_AmmoCheck::CreateTask(UGameplayAbility* owningAbility, TWeakObjectPtr<AItemBase> weaponActor, TWeakObjectPtr<UInventoryComponent> inventoryComponent)
{
	UAT_AmmoCheck* newTask = NewAbilityTask<UAT_AmmoCheck>(owningAbility);
	newTask->armedWeaponActor = weaponActor;
	newTask->playerInventoryComponent = inventoryComponent;

	return newTask;
}

void UAT_AmmoCheck::Activate()
{
	Super::Activate();

	TWeakObjectPtr<UItemProjectileWeapon> weaponObject = Cast<UItemProjectileWeapon>(armedWeaponActor->GetItemObject());
	auto ammoItem = playerInventoryComponent->FindAmmo(weaponObject->GetAmmoType());
	DAmmoCheck.ExecuteIfBound(ammoItem.IsValid());
	
	EndTask();
}

void UAT_AmmoCheck::OnDestroy(bool ability)
{
	Super::OnDestroy(ability);
}

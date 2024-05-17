// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/ItemManager.h"
#include "GameSystem/ActorPooler.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject.h"
#include "Item/ItemObject/ItemProjectileWeapon.h"
#include "Item/ItemObject/ItemMeleeWeapon.h"
#include "Item/ItemObject/ItemRecovery.h"
#include "Item/ItemObject/ItemAmmo.h"
#include "GameSystem/JsonParser.h"
#include "Engine/DataTable.h"
#include "../UntilDawn.h"


UItemManager::UItemManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	itemPooler = CreateDefaultSubobject<UActorPooler>(TEXT("Item Pooler"));
	itemPooler->SetActorClass(AItemBase::StaticClass());

	static ConstructorHelpers::FObjectFinder<UDataTable> Obj_ItemAssetDataTable(TEXT("/Game/_Assets/DataTable/DT_ItemAsset.DT_ItemAsset"));
	if (Obj_ItemAssetDataTable.Succeeded()) itemAssetDataTable = Obj_ItemAssetDataTable.Object;

	jsonParser = MakeShared<JsonParser>();
}

void UItemManager::BeginPlay()
{
	Super::BeginPlay();

}

FItemAsset UItemManager::GetItemAssetMap(const int itemKey)
{
	FItemAsset* itemAsset = itemAssetDataTable->FindRow<FItemAsset>(*FString::FromInt(itemKey), TEXT(""));
	check(itemAsset);
	return *itemAsset;
}

TWeakObjectPtr<UItemObject> UItemManager::CreatePlayersPossessedItem(const PossessedItem& possessed)
{
	auto itemObj = CreateItemObject(FString(UTF8_TO_TCHAR(possessed.itemID.c_str())) , possessed.itemKey);
	itemObj->SetItemQuantity(possessed.quantity);
	if (possessed.isRotated) itemObj->Rotate();
	return itemObj;
}

TWeakObjectPtr<AItemBase> UItemManager::CreatePlayersEquippedItem(const EquippedItem& equipped)
{
	auto itemObj = CreateItemObject(FString(UTF8_TO_TCHAR(equipped.itemID.c_str())), equipped.itemKey);
	TWeakObjectPtr<AItemBase> itemActor = Cast<AItemBase>(itemPooler->GetPooledActor());
	itemActor->SetItemObject(itemObj);
	return itemActor;
}

void UItemManager::SpawnItem(const FString& itemID, const int itemKey, const FVector location)
{
	auto itemObj = CreateItemObject(itemID, itemKey);

	auto actor = itemPooler->GetPooledActor();
	if (actor.IsValid() == false)
	{
		itemPooler->SpawnPoolableActor(10);
		actor = itemPooler->GetPooledActor();
	}

	TWeakObjectPtr<AItemBase> itemActor = Cast<AItemBase>(actor);
	check(itemActor.IsValid());
	itemActor->SetItemObject(itemObj);
	itemActor->ActivateActor();
	itemActor->SetActorLocation(location);

	itemActorMap.Add(itemID, itemActor.Get());
}

TWeakObjectPtr<UItemObject> UItemManager::CreateItemObject(const FString& itemID, const int itemKey)
{
	const FItemInfo& itemInfo = jsonParser->GetItemCommonInfo(itemKey);
	UItemObject* itemObj = NewItemObject(itemInfo.itemType);
	check(itemObj);
	itemObj->Init(itemID, itemInfo, jsonParser->GetItemConcreteInfoMap(itemKey), GetItemAssetMap(itemKey));
	itemObjectMap.Add(itemID, itemObj);
	return itemObj;
}

UItemObject* UItemManager::NewItemObject(const EItemMainType itemType)
{
	switch (itemType)
	{
		case EItemMainType::MeleeWeapon:
			return NewObject<UItemMeleeWeapon>(GetWorld());
		case EItemMainType::RangedWeapon:
			return NewObject<UItemProjectileWeapon>(GetWorld());
		case EItemMainType::RecoveryItem:
			return NewObject<UItemRecovery>(GetWorld());
		case EItemMainType::AmmoItem:
			return NewObject<UItemAmmo>(GetWorld());
		default:
			return nullptr;
	}
}

TWeakObjectPtr<AItemBase> UItemManager::FindItemActor(TWeakObjectPtr<UItemObject> itemObj)
{
	return Cast<AItemBase>(itemPooler->GetPooledActor());
}

TWeakObjectPtr<UItemObject> UItemManager::GetItemObject(const FString& itemID)
{
	if (itemObjectMap.Find(itemID))
	{
		return itemObjectMap[itemID];
	}
	else
	{
		return nullptr;
	}
}

TWeakObjectPtr<AItemBase> UItemManager::GetItemActor(const FString& itemID)
{
	auto itemObj = GetItemObject(itemID);
	return GetItemActor(itemObj);
}

TWeakObjectPtr<AItemBase> UItemManager::GetItemActor(TWeakObjectPtr<UItemObject> itemObj)
{
	TWeakObjectPtr<AItemBase> itemActor = Cast<AItemBase>(itemPooler->GetPooledActor());

	if (itemActor.IsValid())
	{
		itemActor->SetItemObject(itemObj);
		itemActor->ActivateEquipMode();
		itemActorMap.Add(itemObj->GetItemID(), itemActor);
		return itemActor;
	}

	return nullptr;
}

TWeakObjectPtr<AItemBase> UItemManager::GetItemActorInField(const FString& itemID)
{
	if (itemActorMap.Find(itemID))
	{
		return itemActorMap[itemID];
	}
	else
	{
		return nullptr;
	}
}

void UItemManager::ItemEquipped(const FString& itemID, TWeakObjectPtr<AItemBase> itemActor)
{
	itemActorMap.Add(itemID, itemActor);
	itemActor->ActivateEquipMode();
}

void UItemManager::ItemPickedUp(const FString& itemID)
{
	if (itemActorMap.Find(itemID))
	{
		itemActorMap[itemID]->DeactivateActor();
		itemActorMap.Remove(itemID);
	}
}

void UItemManager::ItemPickedUpOtherPlayer(const FString& itemID)
{
	if (itemActorMap.Find(itemID))
	{
		itemActorMap[itemID]->DeactivateActor();
		itemActorMap.Remove(itemID);
	}
}

TWeakObjectPtr<AItemBase> UItemManager::DropItem(TWeakObjectPtr<UItemObject> droppedItemObj)
{
	TWeakObjectPtr<AItemBase> itemActor = GetItemActor(droppedItemObj->GetItemID());
	if (itemActor.IsValid()) 
	{
		itemActor->SetItemObject(droppedItemObj);
		itemActorMap.Add(droppedItemObj->GetItemID(), itemActor.Get());
		return itemActor;
	}
	return nullptr;
}

void UItemManager::DestroyItem(const FString& itemID)
{
	// tweakobject로 파괴되었는지 체크
	itemObjectMap.Remove(itemID);
}


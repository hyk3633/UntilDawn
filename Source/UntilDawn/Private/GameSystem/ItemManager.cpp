// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/ItemManager.h"
#include "GameSystem/ActorPooler.h"
#include "Item/ItemBase.h"
#include "Item/ItemObject.h"
#include "Item/ItemObject/ItemProjectileWeapon.h"
#include "Item/ItemObject/ItemMeleeWeapon.h"
#include "Item/ItemObject/ItemRecovery.h"
#include "Item/ItemObject/ItemAmmo.h"
#include "Item/ItemObject/ItemArmour.h"
#include "Player/PlayerCharacter.h"
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

TWeakObjectPtr<UItemObject> UItemManager::GetPlayersPossessedItem(const PossessedItem& possessed)
{
	const FString itemID = FString(UTF8_TO_TCHAR(possessed.itemID.c_str()));
	auto itemObj = GetItemObject(itemID);
	if (itemObj.IsValid() == false)
	{
		itemObj = CreateItemObject(itemID, possessed.itemKey);
	}
	itemObj->SetItemQuantity(possessed.quantity);
	ItemPickedUp(itemID);
	if (possessed.isRotated)
	{
		itemObj->Rotate();
	}
	return itemObj;
}

TWeakObjectPtr<AItemBase> UItemManager::GetPlayersEquippedItem(const EquippedItem& equipped)
{
	const FString itemID = FString(UTF8_TO_TCHAR(equipped.itemID.c_str()));

	auto itemActor = GetItemActorInField(itemID);
	if (itemActor.IsValid())
		return itemActor;

	auto itemObj = GetItemObject(itemID);
	if (itemObj.IsValid() == false)
	{
		itemObj = CreateItemObject(itemID, equipped.itemKey);
	}
	itemActor = Cast<AItemBase>(itemPooler->GetPooledActor());
	itemActor->SetItemObject(itemObj);
	return itemActor;
}

TWeakObjectPtr<AItemBase> UItemManager::SpawnItem(const FString& itemID, const int itemKey)
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
	itemActor->ActivateFieldMode();

	itemActorMap.Add(itemID, itemActor.Get());

	return itemActor;
}

TWeakObjectPtr<UItemObject> UItemManager::CreateItemObject(const FString& itemID, const int itemKey)
{
	const FItemInfo& itemInfo = jsonParser->GetItemCommonInfo(itemKey);
	UItemObject* itemObj = NewItemObject(itemInfo.itemType);
	check(itemObj);
	itemObj->Init(itemID, itemInfo, jsonParser->GetItemConcreteInfoMap(itemKey), GetItemAssetMap(itemKey));
	if (itemObj->GetItemInfo().isConsumable)
	{
		TWeakObjectPtr<UItemConsumable> itemConsumable = Cast<UItemConsumable>(itemObj);
		itemConsumable->DItemExhaust.BindUFunction(this, FName("ItemExhausted"));
	}
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
		case EItemMainType::ArmourItem:
			return NewObject<UItemArmour>(GetWorld());
		default:
			return nullptr;
	}
}

TWeakObjectPtr<AItemBase> UItemManager::FindItemActor(TWeakObjectPtr<UItemObject> itemObj)
{
	return Cast<AItemBase>(itemPooler->GetPooledActor());
}

TWeakObjectPtr<UItemObject> UItemManager::GetItemObject(const FString& itemID, const int itemKey, const int itemQuantity)
{
	auto itemObj = GetItemObject(itemID);
	if (itemObj.IsValid() == false)
	{
		itemObj = CreateItemObject(itemID, itemKey);
		itemObj->SetItemQuantity(itemQuantity);
		return itemObj;
	}
	else
	{
		return itemObj;
	}
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

TWeakObjectPtr<AItemBase> UItemManager::GetItemActor(const FString& itemID, const int itemKey, const int itemQuantity)
{
	auto itemObj = GetItemObject(itemID, itemKey, itemQuantity);
	return GetItemActor(itemObj);
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

void UItemManager::ItemEquipped(const int playerNumber, const FString& itemID, TWeakObjectPtr<AItemBase> itemActor)
{
	itemActorMap.Add(itemID, itemActor);
	AddToPossessedItemsMap(playerNumber, itemActor);
}

void UItemManager::AddToPossessedItemsMap(const int playerNumber, TWeakObjectPtr<AItemBase> itemActor)
{
	if (equippedItemsMap.Find(playerNumber) == nullptr)
	{
		equippedItemsMap.Add(playerNumber, TArray<TWeakObjectPtr<AItemBase>>());
	}
	equippedItemsMap[playerNumber].Add(itemActor);
}

void UItemManager::ItemPickedUp(const FString& itemID)
{
	if (itemActorMap.Find(itemID))
	{
		itemActorMap[itemID]->DeactivateActor();
		itemActorMap.Remove(itemID);
	}
}

void UItemManager::AddToEquippedItemsMap(const int playerNumber, TWeakObjectPtr<UItemObject> itemObj)
{
	if (possessedItemsMap.Find(playerNumber) == nullptr)
	{
		possessedItemsMap.Add(playerNumber, TArray<TWeakObjectPtr<UItemObject>>());
	}
	possessedItemsMap[playerNumber].Add(itemObj);
}

void UItemManager::ItemPickedUpOtherPlayer(TWeakObjectPtr<APlayerCharacter> player, const FString& itemID)
{
	if (itemActorMap.Find(itemID))
	{
		auto itemObj = itemActorMap[itemID]->GetItemObject();
		itemObj->SetOwnerCharacter(player);

		AddToEquippedItemsMap(player->GetPlayerNumber(), itemObj);

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
	if (itemObjectMap.Find(itemID))
	{
		itemObjectMap[itemID]->MarkAsGarbage();
		itemObjectMap.Remove(itemID);
	}
}

void UItemManager::OtherPlayerUseItem(TWeakObjectPtr<APlayerCharacter> player, const FString& itemID, const int consumedAmount)
{
	if (itemObjectMap.Find(itemID))
	{
		TWeakObjectPtr<UItemObject> itemObj = itemObjectMap[itemID];
		if (itemObj->GetIsConsumable())
		{
			TWeakObjectPtr<UItemConsumable> itemConsumable = Cast<UItemConsumable>(itemObj);
			itemConsumable->Using(consumedAmount);
		}
	}
}

void UItemManager::RemovePlayersItems(const int playerNumber)
{
	if (possessedItemsMap.Find(playerNumber))
	{
		for (auto itemObj : possessedItemsMap[playerNumber])
		{
			if (itemObj.IsValid() == false)
				continue;
			DestroyItem(itemObj->GetItemID());
		}
		possessedItemsMap.Remove(playerNumber);
	}
	if (equippedItemsMap.Find(playerNumber))
	{
		for (auto itemActor : equippedItemsMap[playerNumber])
		{
			DestroyItem(itemActor->GetItemID());
			itemActor->DeactivateActor();
		}
		equippedItemsMap.Remove(playerNumber);
	}
}

void UItemManager::ItemExhausted(const FString& itemID)
{
	DestroyItem(itemID);
}


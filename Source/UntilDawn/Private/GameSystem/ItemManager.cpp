// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/ItemManager.h"
#include "GameSystem/ActorPooler.h"
#include "Item/ItemBase.h"
#include "Item/Weapon/ItemMeleeWeapon.h"
#include "Item/Weapon/ItemRangedWeapon.h"
#include "Item/ItemObject.h"
#include "Engine/DataTable.h"

UItemManager::UItemManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	for (int i = 0; i < static_cast<int>(EItemMainType::MAX); i++)
	{
		itemPoolerMap.Add(i, CreateDefaultSubobject<UActorPooler>(FName(*FString::Printf(TEXT("Pooler %d"), i))));
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> Obj_ItemAssetDataTable(TEXT("/Game/_Assets/DataTable/DT_ItemAsset.DT_ItemAsset"));
	if (Obj_ItemAssetDataTable.Succeeded()) itemAssetDataTable = Obj_ItemAssetDataTable.Object;
}

void UItemManager::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnItems();
	InitializeJson();
	FillItemAssetMap();
}

void UItemManager::SpawnItems()
{
	for (int i = 0; i < static_cast<int>(EItemMainType::MAX); i++)
	{
		itemPoolerMap[i]->SpawnPoolableActor(GetItemClass(static_cast<EItemMainType>(i)), 10);
	}
}

UClass* UItemManager::GetItemClass(EItemMainType type)
{
	switch (type)
	{
	case EItemMainType::MeleeWeapon:
		return AItemMeleeWeapon::StaticClass();
	case EItemMainType::RangedWeapon:
		return AItemRangedWeapon::StaticClass();
	case EItemMainType::RecoveryItem:
		return AItemRangedWeapon::StaticClass();
	case EItemMainType::AmmoItem:
		return AItemRangedWeapon::StaticClass();
	}
	return nullptr;
}

void UItemManager::InitializeJson()
{
	FString fileStr;
	FString filePath = TEXT("D:\\UE5Projects\\UntilDawn\\Json\\ItemInfo.json");
	FFileHelper::LoadFileToString(fileStr, *filePath);

	const TArray<TSharedPtr<FJsonValue>>* jsonItems;

	const TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(fileStr);
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	const bool result = FJsonSerializer::Deserialize(jsonReader, jsonObj);
	check(result);
	check(jsonObj);
	const bool getArray = jsonObj->TryGetArrayField(TEXT("Items"), jsonItems);
	check(getArray);

	ReadJson(jsonItems);
}

void UItemManager::ReadJson(const TArray<TSharedPtr<FJsonValue>>* jsonItems)
{
	for (int i = 0; i < jsonItems->Num(); i++)
	{
		const TSharedPtr<FJsonObject> jsonItem = jsonItems->GetData()[i]->AsObject();

		int itemType;
		jsonItem->TryGetNumberField(TEXT("ItemType"), itemType);
		FItemInfo* itemInfoShPtr = nullptr;
		EItemMainType mainType = StaticCast<EItemMainType>(itemType);

		if (mainType == EItemMainType::MeleeWeapon)
		{
			itemInfoShPtr = new FMeleeWeaponInfo();
		}
		else if (mainType == EItemMainType::RangedWeapon)
		{
			itemInfoShPtr = new FRangedWeaponInfo();
		}
		else if (mainType == EItemMainType::RecoveryItem)
		{
			itemInfoShPtr = new FRecoveryItemInfo();
		}
		else if (mainType == EItemMainType::AmmoItem)
		{
			itemInfoShPtr = new FAmmoItemInfo();
		}
		itemInfoShPtr->itemType = mainType;

		jsonItem->TryGetNumberField(TEXT("ItemKey"), itemInfoShPtr->itemKey);
		jsonItem->TryGetStringField(TEXT("ItemName"), itemInfoShPtr->itemName);

		const TSharedPtr<FJsonObject>* itemGridSizeJsonObject;
		jsonItem->TryGetObjectField(TEXT("ItemGridSize"), itemGridSizeJsonObject);
		itemGridSizeJsonObject->Get()->TryGetNumberField(TEXT("X"), itemInfoShPtr->itemGridSize.X);
		itemGridSizeJsonObject->Get()->TryGetNumberField(TEXT("Y"), itemInfoShPtr->itemGridSize.Y);

		if (itemInfoShPtr->itemType == EItemMainType::MeleeWeapon)
		{
			FMeleeWeaponInfo* meleeWeaponInfo = static_cast<FMeleeWeaponInfo*>(itemInfoShPtr);

			const TSharedPtr<FJsonObject>* meleeWeaponJsonObject;
			jsonItem->TryGetObjectField(TEXT("MeleeWeapon"), meleeWeaponJsonObject);

			meleeWeaponJsonObject->Get()->TryGetNumberField(TEXT("AttackPower"), meleeWeaponInfo->attackPower);
			meleeWeaponJsonObject->Get()->TryGetNumberField(TEXT("AttackSpeed"), meleeWeaponInfo->attackSpeed);
		}
		else if (itemInfoShPtr->itemType == EItemMainType::RangedWeapon)
		{
			FRangedWeaponInfo* rangedWeaponInfo = static_cast<FRangedWeaponInfo*>(itemInfoShPtr);

			const TSharedPtr<FJsonObject>* rangedWeaponJsonObject;
			jsonItem->TryGetObjectField(TEXT("RangedWeapon"), rangedWeaponJsonObject);

			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("AttackPower"), rangedWeaponInfo->attackPower);
			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("FireRate"), rangedWeaponInfo->fireRate);
			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("Recoil"), rangedWeaponInfo->recoil);
			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("Magazine"), rangedWeaponInfo->magazine);
			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("ReloadingSpeed"), rangedWeaponInfo->reloadingSpeed);
		}
		else if (itemInfoShPtr->itemType == EItemMainType::RecoveryItem)
		{
			FRecoveryItemInfo* recoveryItemInfo = static_cast<FRecoveryItemInfo*>(itemInfoShPtr);

			const TSharedPtr<FJsonObject>* recoveryItemJsonObject;
			jsonItem->TryGetObjectField(TEXT("RecoveryItem"), recoveryItemJsonObject);

			recoveryItemJsonObject->Get()->TryGetNumberField(TEXT("RecoveryAmount"), recoveryItemInfo->recoveryAmount);
			recoveryItemJsonObject->Get()->TryGetNumberField(TEXT("UsingSpeed"), recoveryItemInfo->usingSpeed);
		}
		else if (itemInfoShPtr->itemType == EItemMainType::AmmoItem)
		{
			FAmmoItemInfo* ammoItemInfo = static_cast<FAmmoItemInfo*>(itemInfoShPtr);

			const TSharedPtr<FJsonObject>* ammoItemJsonObject;
			jsonItem->TryGetObjectField(TEXT("AmmoItem"), ammoItemJsonObject);

			ammoItemJsonObject->Get()->TryGetNumberField(TEXT("AmmoType"), ammoItemInfo->ammoType);
			ammoItemJsonObject->Get()->TryGetNumberField(TEXT("Amount"), ammoItemInfo->amount);
		}
		itemInfoMap.Add(itemInfoShPtr->itemKey, itemInfoShPtr);
	}
}

void UItemManager::FillItemAssetMap()
{
	for (auto& kv : itemInfoMap)
	{
		FItemAsset* itemAsset = itemAssetDataTable->FindRow<FItemAsset>(*FString::FromInt(StaticCast<int>(kv.Key)), TEXT(""));
		itemAssetMap.Add(kv.Key, itemAsset);
	}
}

void UItemManager::GetData(const int itemKey, FItemInfo* newInfo)
{
	if (itemInfoMap.Find(itemKey) == nullptr)
	{
		return;
	}
	else
	{
		itemInfoMap[itemKey]->CopyTo(newInfo);
	}
}

void UItemManager::ItemPicked(const int itemID)
{
	itemInFieldMap[itemID]->DeactivateActor();
	itemInFieldMap.Remove(itemID);
}

void UItemManager::SpawnItem(const int itemID, const int itemKey, const FVector location)
{

	if (itemInfoMap.Find(itemKey) && itemAssetMap.Find(itemKey))
	{
		UItemObject* itemObj = NewObject<UItemObject>(GetWorld());
		itemObj->DItemPicked.BindUFunction(this, FName("ItemPicked"));
		itemObjectMap.Add(itemID, itemObj);
		itemObj->Init(itemID, itemInfoMap[itemKey], itemAssetMap[itemKey]);

		TWeakObjectPtr<AItemBase> itemActor = Cast<AItemBase>(itemPoolerMap[itemInfoMap[itemKey]->itemKey]->GetPooledActor());
		if (itemActor.IsValid())
		{
			itemInFieldMap.Add(itemID, itemActor);
			itemActor->SetItemObject(itemObj);
			itemActor->ActivateActor();
			itemActor->SetActorLocation(location);
		}
	}
}

TWeakObjectPtr<UItemObject> UItemManager::GetItemObject(const int itemID)
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

void UItemManager::ItemPickUpOtherPlayer(const int itemID)
{
	itemObjectPickedMap.Add(itemID, itemInFieldMap[itemID]->GetItemObject());
	itemInFieldMap[itemID]->DeactivateActor();
	itemInFieldMap.Remove(itemID);
}

TWeakObjectPtr<AItemBase> UItemManager::DropItem(TWeakObjectPtr<UItemObject> droppedItemObj)
{
	AItemBase* item = Cast<AItemBase>(itemPoolerMap[droppedItemObj->GetItemType()]->GetPooledActor());
	item->SetItemObject(droppedItemObj);
	itemInFieldMap.Add(droppedItemObj->GetItemID(), item);
	itemObjectPickedMap.Remove(droppedItemObj->GetItemID());
	return item;
}

void UItemManager::DestroyItem(const int itemID)
{
	// tweakobject로 파괴되었는지 체크
	itemObjectMap.Remove(itemID);
	itemObjectPickedMap.Remove(itemID);
}


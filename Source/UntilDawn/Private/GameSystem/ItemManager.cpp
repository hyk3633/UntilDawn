// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/ItemManager.h"
#include "GameSystem/ActorPooler.h"
#include "Item/ItemBase.h"
#include "Item/Weapon/ItemWeapon.h"
#include "Item/Weapon/ItemMeleeWeapon.h"
#include "Item/Weapon/ItemRangedWeapon.h"
#include "Item/Consumable/ItemRecovery.h"
#include "Item/Consumable/ItemAmmo.h"
#include "Item/ItemObject.h"
#include "Engine/DataTable.h"
#include "../UntilDawn.h"


UItemManager::UItemManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	itemPooler = CreateDefaultSubobject<UActorPooler>(TEXT("Item Pooler"));

	static ConstructorHelpers::FObjectFinder<UDataTable> Obj_ItemAssetDataTable(TEXT("/Game/_Assets/DataTable/DT_ItemAsset.DT_ItemAsset"));
	if (Obj_ItemAssetDataTable.Succeeded()) itemAssetDataTable = Obj_ItemAssetDataTable.Object;
}

void UItemManager::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeJson();

	for (int i = 0; i < 4; i++)
	{
		SpawnItem(i, i, FVector::ZeroVector);
	}

	for (auto& kv : tempMap2)
	{
		switch (kv.Key)
		{
			case 0:
			{
				//FMemoryReader FromBinary = FMemoryReader(kv.Value.buff);
				//FWeaponInfo weaponInfo;
				//FromBinary << weaponInfo.attackPower;
				//FromBinary << weaponInfo.weaponType;
				//PLOG(TEXT("melee %f %d"), weaponInfo.attackPower, weaponInfo.weaponType);
				//break;
			}
			case 1:
			{
				//FMemoryReader FromBinary = FMemoryReader(kv.Value.get)
				break;
			}
			case 2:
			{
				//FMemoryReader FromBinary = FMemoryReader(kv.Value.buff);
				//FRecoveryItemInfo rinfo;
				//FromBinary << rinfo.recoveryAmount;
				//FromBinary << rinfo.usingSpeed;
				//PLOG(TEXT("recovery %d %f"), rinfo.recoveryAmount, rinfo.usingSpeed);
				//break;
			}
			case 3:
			{
				break;
			}
		}
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
		return AItemRecovery::StaticClass();
	case EItemMainType::AmmoItem:
		return AItemAmmo::StaticClass();
	}
	return nullptr;
}

UClass* UItemManager::GetWeaponClass(EWeaponType type)
{
	switch (type)
	{
	case EWeaponType::AXE:
		return AItemMeleeWeapon::StaticClass();
	case EWeaponType::BOW:
		return AItemRangedWeapon::StaticClass();
	}
	return nullptr;
}

void UItemManager::InitializeJson()
{
	FString fileStr;
	FString filePath = TEXT("D:\\UE5Projects\\UntilDawn\\Json\\ItemInfo.json");
	FFileHelper::LoadFileToString(fileStr, *filePath);

	const TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(fileStr);
	jsonObj = MakeShareable(new FJsonObject());

	const bool result = FJsonSerializer::Deserialize(jsonReader, jsonObj);
	check(result);
	check(jsonObj);
}

FItemInfo UItemManager::GetItemCommonInfo(const int itemKey)
{
	const TSharedPtr<FJsonObject>* itemJson;
	if (jsonObj->TryGetObjectField(*FString::FromInt(itemKey), itemJson))
	{
		FItemInfo itemInfo;
		SaveItemCommonInfo(itemJson, itemInfo);
		return itemInfo;
	}
	return FItemInfo();
}

void UItemManager::SaveItemCommonInfo(const TSharedPtr<FJsonObject>* jsonItem, FItemInfo& itemInfo)
{
	int itemType;
	jsonItem->Get()->TryGetNumberField(TEXT("ItemKey"), itemInfo.itemKey);
	jsonItem->Get()->TryGetStringField(TEXT("ItemName"), itemInfo.itemName);
	jsonItem->Get()->TryGetNumberField(TEXT("ItemType"), itemType);
	jsonItem->Get()->TryGetBoolField(TEXT("IsConsumable"), itemInfo.isConsumable);
	itemInfo.itemType = static_cast<EItemMainType>(itemType);

	const TSharedPtr<FJsonObject>* itemGridSizeJsonObject;
	jsonItem->Get()->TryGetObjectField(TEXT("ItemGridSize"), itemGridSizeJsonObject);
	itemGridSizeJsonObject->Get()->TryGetNumberField(TEXT("X"), itemInfo.itemGridSize.X);
	itemGridSizeJsonObject->Get()->TryGetNumberField(TEXT("Y"), itemInfo.itemGridSize.Y);

	//PLOG(TEXT("item : %d %s %d , grid : %d %d"), itemInfo.itemKey, *itemInfo.itemName, itemInfo.itemType, itemInfo.itemGridSize.X, itemInfo.itemGridSize.Y);
}

void UItemManager::SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FWeaponInfo& itemInfo)
{
	jsonItem->Get()->TryGetNumberField(TEXT("AttackPower"), itemInfo.attackPower);
	jsonItem->Get()->TryGetNumberField(TEXT("WeaponType"), itemInfo.weaponType);
}

void UItemManager::SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FRangedWeaponInfo& itemInfo)
{
	jsonItem->Get()->TryGetNumberField(TEXT("AttackPower"),	itemInfo.attackPower);
	jsonItem->Get()->TryGetNumberField(TEXT("WeaponType"),	itemInfo.weaponType);

	const TSharedPtr<FJsonObject>* rangedJson;
	jsonItem->Get()->TryGetObjectField(TEXT("RangedWeapon"), rangedJson);

	rangedJson->Get()->TryGetNumberField(TEXT("FireRate"),			itemInfo.fireRate);
	rangedJson->Get()->TryGetNumberField(TEXT("Recoil"),			itemInfo.recoil);
	rangedJson->Get()->TryGetNumberField(TEXT("Magazine"),			itemInfo.magazine);
	rangedJson->Get()->TryGetNumberField(TEXT("ReloadingSpeed"),	itemInfo.reloadingSpeed);

	//PLOG(TEXT("ranged : %f %d , %f %f %d %f"), itemInfo.attackPower, itemInfo.weaponType, itemInfo.fireRate, itemInfo.recoil, itemInfo.magazine, itemInfo.reloadingSpeed);
}

void UItemManager::SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FRecoveryItemInfo& itemInfo)
{
	const TSharedPtr<FJsonObject>* recoveryJson;
	jsonItem->Get()->TryGetObjectField(TEXT("RecoveryItem"), recoveryJson);

	recoveryJson->Get()->TryGetNumberField(TEXT("RecoveryAmount"),	itemInfo.recoveryAmount);
	recoveryJson->Get()->TryGetNumberField(TEXT("UsingSpeed"),		itemInfo.usingSpeed);

	//PLOG(TEXT("recovery : %d %f"), itemInfo.recoveryAmount, itemInfo.usingSpeed);
}

void UItemManager::SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FAmmoItemInfo& itemInfo)
{
	const TSharedPtr<FJsonObject>* ammoJson;
	jsonItem->Get()->TryGetObjectField(TEXT("AmmoItem"), ammoJson);

	ammoJson->Get()->TryGetNumberField(TEXT("AmmoType"), itemInfo.ammoType);
	ammoJson->Get()->TryGetNumberField(TEXT("Amount"), itemInfo.amount);

	//PLOG(TEXT("ammo : %d %d"), itemInfo.ammoType, itemInfo.amount);
}

bool UItemManager::GetItemAssetMap(const int itemKey)
{
	FItemAsset* itemAsset = itemAssetDataTable->FindRow<FItemAsset>(*FString::FromInt(itemKey), TEXT(""));
	if (itemAsset == nullptr) return false;
	itemAssetMap.Add(itemKey, *itemAsset);
	return true;
}

void UItemManager::GetData(const int itemKey, FItemInfo* newInfo)
{
	if (itemInfoMap.Find(itemKey) == nullptr)
	{
		return;
	}
	else
	{
		//itemInfoMap[itemKey]->CopyTo(newInfo);
	}
}

void UItemManager::SpawnItem(const int itemID, const int itemKey, const FVector location)
{
	UItemObject* itemObj = NewObject<UItemObject>(GetWorld());
	itemObjectMap.Add(itemID, itemObj);
	if (itemAssetMap.Find(itemKey) == nullptr)
	{
		const bool assetResult = GetItemAssetMap(itemKey);
		check(assetResult);
	}
	itemObj->Init(itemID, GetItemCommonInfo(itemKey), itemAssetMap[itemKey]);

	TWeakObjectPtr<AItemBase> itemActor;
	if (itemObj->GetIsConsumable())
	{
		const int type = itemObj->GetItemType();
		if (itemPooler->IsActorTypeExist(type) == false)
		{
			itemPooler->SpawnPoolableActor(type, GetItemClass(static_cast<EItemMainType>(type)), 10);
		}
		itemActor = Cast<AItemBase>(itemPooler->GetPooledActor(type));
	}
	else
	{
		AItemBase* itemBase = GetWorld()->SpawnActor<AItemBase>(GetItemClass(static_cast<EItemMainType>(itemObj->GetItemType())), FVector(0, 0, -3500), FRotator::ZeroRotator);
		//nonConsItemMap.Add(itemID, itemBase);
		itemActor = itemBase;
	}

	if (itemActor.IsValid())
	{
		//itemInFieldMap.Add(itemID, itemActor);
		itemActor->SetItemObject(itemObj);
		InitializeItemSpecificInfo(itemActor, itemKey);
		itemActor->ActivateActor();
		itemActor->SetActorLocation(location);
	}

	itemActorMap.Add(itemID, itemActor.Get());

	//FItemInfo info = itemActor->GetItemObject()->GetItemInfo();
	//PLOG(TEXT("item : %d %s %d , grid : %d %d"), info.itemKey, *info.itemName, info.itemType, info.itemGridSize.X, info.itemGridSize.Y);

	/*switch (info.itemType)
	{
		case EItemMainType::MeleeWeapon:
		{
			AItemMeleeWeapon* meleeWeapon = Cast<AItemMeleeWeapon>(itemActor.Get());
			PLOG(TEXT("melee %f %d"), meleeWeapon->GetWeaponInfo().attackPower, meleeWeapon->GetWeaponInfo().weaponType);
			break;
		}
		case EItemMainType::RangedWeapon:
		{
			AItemRangedWeapon* rangedWeapon = Cast<AItemRangedWeapon>(itemActor.Get());
			PLOG(TEXT("ranged %f %d %f %d %f %f"), rangedWeapon->GetRangedInfo().attackPower, rangedWeapon->GetRangedInfo().weaponType,
				rangedWeapon->GetRangedInfo().fireRate, rangedWeapon->GetRangedInfo().magazine, 
				rangedWeapon->GetRangedInfo().recoil, rangedWeapon->GetRangedInfo().reloadingSpeed);
			break;
		}
		case EItemMainType::RecoveryItem:
		{
			AItemRecovery* recoveryItem = Cast<AItemRecovery>(itemActor.Get());
			PLOG(TEXT("recovery %d %f"), recoveryItem->GetRecoveryInfo().recoveryAmount, recoveryItem->GetRecoveryInfo().usingSpeed);
			break;
		}
		case EItemMainType::AmmoItem:
		{
			AItemAmmo* ammoItem = Cast<AItemAmmo>(itemActor.Get());
			PLOG(TEXT("ammo %d %d"), ammoItem->GetAmmoInfo().ammoType, ammoItem->GetAmmoInfo().amount);
			break;
		}
	}*/
}

void UItemManager::InitializeItemSpecificInfo(TWeakObjectPtr<AItemBase> item, const int itemKey)
{
	EItemMainType itemType = static_cast<EItemMainType>(item->GetItemObject()->GetItemType());
	const TSharedPtr<FJsonObject>* itemJson;
	if (jsonObj->TryGetObjectField(*FString::FromInt(itemKey), itemJson))
	{
		switch (itemType)
		{
			case EItemMainType::MeleeWeapon:
			{
				AItemMeleeWeapon* meleeWeapon = Cast<AItemMeleeWeapon>(item.Get());
				FWeaponInfo weaponInfo;
				SaveItemSpecificInfo(itemJson, weaponInfo);
				meleeWeapon->InitializeWeaponInfo(weaponInfo);

				FBufferArchive buf;
				buf << weaponInfo.attackPower;
				buf << weaponInfo.weaponType;

				tempMap.Add(itemKey, FTemp(buf));

				break;
			}
			case EItemMainType::RangedWeapon:
			{
				AItemRangedWeapon* rangedWeapon = Cast<AItemRangedWeapon>(item.Get());
				FRangedWeaponInfo rangedWeaponInfo;
				SaveItemSpecificInfo(itemJson, rangedWeaponInfo);
				rangedWeapon->InitializeRangedWeaponInfo(rangedWeaponInfo);

				FBufferArchive buf;
				buf << rangedWeaponInfo;

				tempMap.Add(itemKey, FTemp(buf));

				break;
			}
			case EItemMainType::RecoveryItem:
			{
				AItemRecovery* recoveryItem = Cast<AItemRecovery>(item.Get());
				FRecoveryItemInfo recoveryItemInfo;
				SaveItemSpecificInfo(itemJson, recoveryItemInfo);
				recoveryItem->InitializeRecoveryInfo(recoveryItemInfo);

				FBufferArchive buf;
				buf << recoveryItemInfo.recoveryAmount;
				buf << recoveryItemInfo.usingSpeed;

				tempMap.Add(itemKey, FTemp(buf));

				break;
			}
			case EItemMainType::AmmoItem:
			{
				AItemAmmo* ammoItem = Cast<AItemAmmo>(item.Get());
				FAmmoItemInfo ammoItemInfo;
				SaveItemSpecificInfo(itemJson, ammoItemInfo);
				ammoItem->InitializeAmmoInfo(ammoItemInfo);

				FBufferArchive buf;
				buf << ammoItemInfo.ammoType;
				buf << ammoItemInfo.amount;

				tempMap.Add(itemKey, FTemp(buf));

				break;
			}
		}
	}
}

TWeakObjectPtr<AItemBase> UItemManager::FindItemActor(TWeakObjectPtr<UItemObject> itemObj)
{
	if (itemObj->GetIsConsumable())
	{
		return Cast<AItemBase>(itemPooler->GetPooledActor(itemObj->GetItemType()));
	}
	else
	{
		//return nonConsItemMap[itemObj->GetItemID()];
	}
	return nullptr;
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

TWeakObjectPtr<AItemBase> UItemManager::GetItemActor(TWeakObjectPtr<UItemObject> itemObj)
{
	TWeakObjectPtr<AItemBase> itemActor = GetItemActor(itemObj->GetItemID());

	if (itemActor.IsValid())
	{
		itemActor->SetItemObject(itemObj);
		itemActor->ActivateEquipMode();
		return itemActor;
	}

	return nullptr;
}

TWeakObjectPtr<AItemBase> UItemManager::GetItemActor(const int itemID)
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

void UItemManager::ItemPickedUp(const int itemID)
{
	if (itemActorMap.Find(itemID))
	{
		itemActorMap[itemID]->DeactivateActor();
		//itemInFieldMap.Remove(itemID);
	}
}

void UItemManager::ItemPickedUpOtherPlayer(const int itemID)
{
	if (itemActorMap.Find(itemID))
	{
		//itemObjectPickedMap.Add(itemID, itemInFieldMap[itemID]->GetItemObject());
		itemActorMap[itemID]->DeactivateActor();
		//itemInFieldMap.Remove(itemID);
	}
}

TWeakObjectPtr<AItemBase> UItemManager::DropItem(TWeakObjectPtr<UItemObject> droppedItemObj)
{
	TWeakObjectPtr<AItemBase> itemActor = GetItemActor(droppedItemObj->GetItemID());
	if (itemActor.IsValid()) 
	{
		itemActor->SetItemObject(droppedItemObj);
		//itemInFieldMap.Add(droppedItemObj->GetItemID(), itemActor);
		//itemObjectPickedMap.Remove(droppedItemObj->GetItemID());
		return itemActor;
	}
	return nullptr;
}

void UItemManager::DestroyItem(const int itemID)
{
	// tweakobject로 파괴되었는지 체크
	itemObjectMap.Remove(itemID);
	//itemObjectPickedMap.Remove(itemID);
}


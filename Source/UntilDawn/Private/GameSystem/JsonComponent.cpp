
#include "GameSystem/JsonComponent.h"
#include "Item/ItemCore.h"
#include "Engine/DataTable.h"

JsonComponent::JsonComponent()
{

}

JsonComponent::~JsonComponent()
{
}

void JsonComponent::Initialize()
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

void JsonComponent::ReadJson(const TArray<TSharedPtr<FJsonValue>>* jsonItems)
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

			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("AttackPower"),		rangedWeaponInfo->attackPower);
			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("FireRate"),			rangedWeaponInfo->fireRate);
			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("Recoil"),			rangedWeaponInfo->recoil);
			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("Magazine"),			rangedWeaponInfo->magazine);
			rangedWeaponJsonObject->Get()->TryGetNumberField(TEXT("ReloadingSpeed"),	rangedWeaponInfo->reloadingSpeed);
		}
		else if (itemInfoShPtr->itemType == EItemMainType::RecoveryItem)
		{
			FRecoveryItemInfo* recoveryItemInfo = static_cast<FRecoveryItemInfo*>(itemInfoShPtr);

			const TSharedPtr<FJsonObject>* recoveryItemJsonObject;
			jsonItem->TryGetObjectField(TEXT("RecoveryItem"), recoveryItemJsonObject);

			recoveryItemJsonObject->Get()->TryGetNumberField(TEXT("RecoveryAmount"),	recoveryItemInfo->recoveryAmount);
			recoveryItemJsonObject->Get()->TryGetNumberField(TEXT("UsingSpeed"),		recoveryItemInfo->usingSpeed);
		}
		else if (itemInfoShPtr->itemType == EItemMainType::AmmoItem)
		{
			FAmmoItemInfo* ammoItemInfo = static_cast<FAmmoItemInfo*>(itemInfoShPtr);

			const TSharedPtr<FJsonObject>* ammoItemJsonObject;
			jsonItem->TryGetObjectField(TEXT("AmmoItem"), ammoItemJsonObject);

			ammoItemJsonObject->Get()->TryGetNumberField(TEXT("AmmoType"),	ammoItemInfo->ammoType);
			ammoItemJsonObject->Get()->TryGetNumberField(TEXT("Amount"),	ammoItemInfo->amount);
		}
		itemInfoMap.Add(itemInfoShPtr->itemKey, itemInfoShPtr);
	}
}

void JsonComponent::FillItemInfoMap(TMap<int, FItemInfo*>& infoMap)
{
	for (auto& kv : itemInfoMap)
	{
		switch (static_cast<EItemMainType>(kv.Key))
		{
			case EItemMainType::MeleeWeapon:
			{
				FMeleeWeaponInfo* meleeWeaponInfo = new FMeleeWeaponInfo();
				static_cast<FMeleeWeaponInfo*>(kv.Value)->CopyTo(meleeWeaponInfo);
				infoMap.Add(kv.Key, meleeWeaponInfo);
				break;
			}
			case EItemMainType::RangedWeapon:
			{
				FRangedWeaponInfo* rangedWeaponInfo = new FRangedWeaponInfo();
				static_cast<FRangedWeaponInfo*>(kv.Value)->CopyTo(rangedWeaponInfo);
				infoMap.Add(kv.Key, rangedWeaponInfo);
				break;
			}
			case EItemMainType::RecoveryItem:
			{
				FRecoveryItemInfo* recoveryItemInfo = new FRecoveryItemInfo();
				static_cast<FRecoveryItemInfo*>(kv.Value)->CopyTo(recoveryItemInfo);
				infoMap.Add(kv.Key, recoveryItemInfo);
				break;
			}
			case EItemMainType::AmmoItem:
			{
				FAmmoItemInfo* ammoItemInfo = new FAmmoItemInfo();
				static_cast<FAmmoItemInfo*>(kv.Value)->CopyTo(ammoItemInfo);
				infoMap.Add(kv.Key, ammoItemInfo);
				break;
			}
		}
	}
}

void JsonComponent::GetData(const int itemKey, FItemInfo* newInfo)
{
	if (itemInfoMap.Find(itemKey) == nullptr)
	{
		return;
	}
	else
	{
		itemInfoMap[itemKey]->CopyTo(newInfo);
	}

	/*
	for (auto& kv : itemInfoMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("item key %d"), kv.Key);
		UE_LOG(LogTemp, Warning, TEXT("item type %d, item Name %s, grid size %d %d"), 
			kv.Value->itemType, *kv.Value->itemName, kv.Value->itemGridSize.X, kv.Value->itemGridSize.Y);

		if (kv.Value->itemType == EItemMainType::MeleeWeapon)
		{
			const FMeleeWeaponInfo* meleeWeaponInfo = static_cast<FMeleeWeaponInfo*>(kv.Value.Get());
			
			UE_LOG(LogTemp, Warning, TEXT("attack %f %f"), meleeWeaponInfo->attackPower, meleeWeaponInfo->attackSpeed);
		}
		else if (kv.Value->itemType == EItemMainType::RangedWeapon)
		{
			const FRangedWeaponInfo* rangedWeaponInfo = static_cast<FRangedWeaponInfo*>(kv.Value.Get());
			
			UE_LOG(LogTemp, Warning, TEXT("gun %f %f %f %d %f"), 
				rangedWeaponInfo->attackPower, rangedWeaponInfo->fireRate, rangedWeaponInfo->recoil, rangedWeaponInfo->magazine, rangedWeaponInfo->reloadingSpeed);
		}
		else if (kv.Value->itemType == EItemMainType::RecoveryItem)
		{
			const FRecoveryItemInfo* recoveryItemInfo = static_cast<FRecoveryItemInfo*>(kv.Value.Get());
		}
		else if (kv.Value->itemType == EItemMainType::Ammo)
		{
			const FAmmoItemInfo* ammoItemInfo = static_cast<FAmmoItemInfo*>(kv.Value.Get());
		}
	}
	*/
}



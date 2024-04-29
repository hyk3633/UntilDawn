// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Enums/ItemType.h"
#include "../Enums/WeaponType.h"
#include "../Structs/ItemInfo.h"
#include "../Structs/ItemAsset.h"
#include "ItemManager.generated.h"

class UDataTable;
class UActorSpawner;
class UActorPooler;
class AItemBase;
class AItemWeapon;
class UItemObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UItemManager();

protected:

	virtual void BeginPlay() override;

	UClass* GetItemClass(EItemMainType type);

	UClass* GetWeaponClass(EWeaponType type);

	void InitializeJson();

	FItemInfo GetItemCommonInfo(const int itemKey);

	void SaveItemCommonInfo(const TSharedPtr<FJsonObject>* jsonItem, FItemInfo& itemInfo);

	void SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FWeaponInfo& itemInfo);

	void SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FRangedWeaponInfo& itemInfo);

	void SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FRecoveryItemInfo& itemInfo);

	void SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FAmmoItemInfo& itemInfo);

	bool GetItemAssetMap(const int itemKey);

	void GetData(const int itemKey, FItemInfo* newInfo);

	UFUNCTION()
	void ItemPicked(const int itemID);

public:

	void SpawnItem(const int itemID, const int itemKey, const FVector location);

protected:

	void InitializeItemSpecificInfo(TWeakObjectPtr<AItemBase> item, const int itemKey);

	TWeakObjectPtr<AItemBase> FindItemActor(TWeakObjectPtr<UItemObject> itemObj);

public:

	TWeakObjectPtr<UItemObject> GetItemObject(const int itemID);

	TWeakObjectPtr<AItemBase> GetItemActor(TWeakObjectPtr<UItemObject> itemObj);

	// 다른 플레이어가 획득한 경우 아이템 오브젝트를 따로 맵에 저장하고 액터는 풀링
	void ItemPickUpOtherPlayer(const int itemID);

	// 플레이어나 다른 플레이어가 아이템을 버린 경우 아이템 오브젝트를 풀링된 액터에 저장하여 스폰
	TWeakObjectPtr<AItemBase> DropItem(TWeakObjectPtr<UItemObject> droppedItemObj);

	// 다쓴 아이템을 파괴, 아이템 오브젝트는 삭제하고 액터는 풀링
	void DestroyItem(const int itemID);

private:

	UPROPERTY(VisibleAnywhere, Category = "Item Info", meta = (AllowPrivateAccess = "true"))
	UDataTable* itemAssetDataTable;

	TSharedPtr<FJsonObject> jsonObj;

	UPROPERTY()
	UActorPooler* itemPooler;

	UPROPERTY()
	TMap<int, AItemBase*> nonConsItemMap;

	TMap<int, FItemInfo*> itemInfoMap;

	TMap<int, FItemAsset> itemAssetMap;

	TMap<int, TWeakObjectPtr<AItemBase>> itemInFieldMap;

	UPROPERTY()
	TMap<int, UItemObject*> itemObjectMap;

	TMap<int, TWeakObjectPtr<UItemObject>> itemObjectPickedMap;

};

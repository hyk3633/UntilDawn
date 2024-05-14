// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Enums/ItemType.h"
#include "../Enums/WeaponType.h"
#include "../Structs/ItemInfo.h"
#include "../Structs/ItemAsset.h"
#include "../Structs/PossessedItem.h"
#include "../Structs/EquippedItem.h"
#include "ItemManager.generated.h"

class UDataTable;
class UActorSpawner;
class UActorPooler;
class AItemBase;
class AItemWeapon;
class UItemObject;
class JsonParser;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UItemManager();

protected:

	virtual void BeginPlay() override;

	FItemAsset GetItemAssetMap(const int itemKey);

public:

	TWeakObjectPtr<UItemObject> CreatePlayersPossessedItem(const PossessedItem& possessed);

	TWeakObjectPtr<AItemBase> CreatePlayersEquippedItem(const EquippedItem& equipped);

	void SpawnItem(const FString& itemID, const int itemKey, const FVector location);

protected:

	TWeakObjectPtr<UItemObject> CreateItemObject(const FString& itemID, const int itemKey);

	UItemObject* NewItemObject(const EItemMainType itemType);

	TWeakObjectPtr<AItemBase> FindItemActor(TWeakObjectPtr<UItemObject> itemObj);

public:

	TWeakObjectPtr<UItemObject> GetItemObject(const FString& itemID);

	TWeakObjectPtr<AItemBase> GetItemActor(const FString& itemID);

	TWeakObjectPtr<AItemBase> GetItemActor(TWeakObjectPtr<UItemObject> itemObj);

	TWeakObjectPtr<AItemBase> GetItemActorInField(const FString& itemID);

	void ItemEquipped(const FString& itemID, TWeakObjectPtr<AItemBase> itemActor);

	void ItemPickedUp(const FString& itemID);

	// 다른 플레이어가 획득한 경우 아이템 오브젝트를 따로 맵에 저장하고 액터는 풀링
	void ItemPickedUpOtherPlayer(const FString& itemID);

	// 플레이어나 다른 플레이어가 아이템을 버린 경우 아이템 오브젝트를 풀링된 액터에 저장하여 스폰
	TWeakObjectPtr<AItemBase> DropItem(TWeakObjectPtr<UItemObject> droppedItemObj);

	// 다쓴 아이템을 파괴, 아이템 오브젝트는 삭제하고 액터는 풀링
	void DestroyItem(const FString& itemID);

private:

	UPROPERTY(VisibleAnywhere, Category = "Item Info", meta = (AllowPrivateAccess = "true"))
	UDataTable* itemAssetDataTable;

	TSharedPtr<JsonParser> jsonParser;

	UPROPERTY()
	UActorPooler* itemPooler;

	TMap<int, FItemAsset> itemAssetMap;

	UPROPERTY()
	TMap<FString, UItemObject*> itemObjectMap;

	UPROPERTY()
	TMap<FString, TWeakObjectPtr<AItemBase>> itemActorMap;

};

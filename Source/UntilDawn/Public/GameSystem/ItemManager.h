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
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UItemManager();

	/* 아이템 액터 생성 */

	TWeakObjectPtr<AItemBase> SpawnItem(const FString& itemID, const int itemKey);

protected:

	/* 아이템 오브젝트 생성 */

	TWeakObjectPtr<UItemObject> CreateItemObject(const FString& itemID, const int itemKey);

	// NewObject로 아이템 오브젝트 포인터 반환
	UItemObject* NewItemObject(const EItemMainType itemType);

protected:

	FItemAsset GetItemAssetMap(const int itemKey);

public:

	/* 플레이어 아이템 생성 */

	TWeakObjectPtr<UItemObject> GetPlayersPossessedItem(const PossessedItem& possessed);

	TWeakObjectPtr<AItemBase> GetPlayersEquippedItem(const EquippedItem& equipped);

	/* 아이템 가져오기 */

	TWeakObjectPtr<UItemObject> GetItemObject(const FString& itemID, const int itemKey, const int itemQuantity);

	TWeakObjectPtr<UItemObject> GetItemObject(const FString& itemID);

	TWeakObjectPtr<AItemBase> GetItemActor(const FString& itemID, const int itemKey, const int itemQuantity);

	TWeakObjectPtr<AItemBase> GetItemActor(const FString& itemID);

	TWeakObjectPtr<AItemBase> GetItemActor(TWeakObjectPtr<UItemObject> itemObj);

	TWeakObjectPtr<AItemBase> GetItemActorInField(const FString& itemID);

	/* 플레이어 아이템 상호작용 */

	void ItemPickedUp(const FString& itemID);

	// 다른 플레이어가 획득한 경우 아이템 오브젝트를 따로 맵에 저장하고 액터는 풀링
	void ItemPickedUpOtherPlayer(TWeakObjectPtr<APlayerCharacter> player, const FString& itemID);

protected:

	// playerNumber의 플레이어가 획득한 아이템 오브젝트를 맵에 저장
	void AddPossessedItemsToMap(const int playerNumber, TWeakObjectPtr<UItemObject> itemObj);

public:

	void ItemEquipped(const int playerNumber, const FString& itemID, TWeakObjectPtr<AItemBase> itemActor);

protected:

	// playerNumber의 플레이어가 장착한 아이템 액터를 맵에 저장
	void AddEquippedItemsToMap(const int playerNumber, TWeakObjectPtr<AItemBase> itemActor);

public:

	// 플레이어가 아이템을 버린 경우 아이템 오브젝트를 풀링된 액터에 저장하여 스폰
	TWeakObjectPtr<AItemBase> DropItem(TWeakObjectPtr<UItemObject> droppedItemObj);

	void OtherPlayerUseItem(TWeakObjectPtr<APlayerCharacter> player, const FString& itemID, const int consumedAmount);

protected:

	// 다쓴 아이템을 파괴, 아이템 오브젝트는 삭제하고 액터는 풀링
	void DestroyItem(const FString& itemID);

	UFUNCTION()
	void ItemExhausted(const FString& itemID);

public:

	void RemovePlayersItems(const int playerNumber);

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

	TMap<int, TArray<TWeakObjectPtr<UItemObject>>> possessedItemsMap;

	TMap<int, TArray<TWeakObjectPtr<AItemBase>>> equippedItemsMap;

};

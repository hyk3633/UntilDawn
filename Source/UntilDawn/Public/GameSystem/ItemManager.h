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
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/BufferArchive.h"
#include "ItemManager.generated.h"

class UDataTable;
class UActorSpawner;
class UActorPooler;
class AItemBase;
class AItemWeapon;
class UItemObject;

USTRUCT()
struct FTemp
{
	GENERATED_BODY()
public:
	FTemp() {}
	FTemp(FBufferArchive& buf)
	{
		buff.Append(buf.GetData(), buf.Num());
	}

	TArray<uint8> buff;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UItemManager();

protected:

	virtual void BeginPlay() override;

	void InitializeJson();

	FItemInfo GetItemCommonInfo(const int itemKey);

	void SaveItemCommonInfo(const TSharedPtr<FJsonObject>* jsonItem, FItemInfo& itemInfo);

	void SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FWeaponInfo& itemInfo);

	void SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FRangedWeaponInfo& itemInfo);

	void SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FRecoveryItemInfo& itemInfo);

	void SaveItemSpecificInfo(const TSharedPtr<FJsonObject>* jsonItem, FAmmoItemInfo& itemInfo);

	FItemAsset GetItemAssetMap(const int itemKey);

	void GetData(const int itemKey, FItemInfo* newInfo);

public:

	TWeakObjectPtr<UItemObject> CreatePlayersPossessedItem(const PossessedItem& possessed);

	TWeakObjectPtr<AItemBase> CreatePlayersEquippedItem(const EquippedItem& equipped);

	void SpawnItem(const FString& itemID, const int itemKey, const FVector location);

protected:

	TWeakObjectPtr<UItemObject> CreateItemObject(const FString& itemID, const int itemKey);

	void InitializeItemSpecificInfo(TWeakObjectPtr<AItemBase> item, const int itemKey);

	TWeakObjectPtr<AItemBase> FindItemActor(TWeakObjectPtr<UItemObject> itemObj);

public:

	TWeakObjectPtr<UItemObject> GetItemObject(const FString& itemID);

	TWeakObjectPtr<AItemBase> GetItemActor(const FString& itemID);

	TWeakObjectPtr<AItemBase> GetItemActor(TWeakObjectPtr<UItemObject> itemObj);

	TWeakObjectPtr<AItemBase> GetItemActorInField(const FString& itemID);

	void ItemEquipped(const FString& itemID, TWeakObjectPtr<AItemBase> itemActor);

	void ItemPickedUp(const FString& itemID);

	// �ٸ� �÷��̾ ȹ���� ��� ������ ������Ʈ�� ���� �ʿ� �����ϰ� ���ʹ� Ǯ��
	void ItemPickedUpOtherPlayer(const FString& itemID);

	// �÷��̾ �ٸ� �÷��̾ �������� ���� ��� ������ ������Ʈ�� Ǯ���� ���Ϳ� �����Ͽ� ����
	TWeakObjectPtr<AItemBase> DropItem(TWeakObjectPtr<UItemObject> droppedItemObj);

	// �پ� �������� �ı�, ������ ������Ʈ�� �����ϰ� ���ʹ� Ǯ��
	void DestroyItem(const FString& itemID);

private:

	UPROPERTY(VisibleAnywhere, Category = "Item Info", meta = (AllowPrivateAccess = "true"))
	UDataTable* itemAssetDataTable;

	TSharedPtr<FJsonObject> jsonObj;

	UPROPERTY()
	UActorPooler* itemPooler;

	//UPROPERTY()
	//TMap<int, AItemBase*> nonConsItemMap;

	TMap<int, FItemInfo*> itemInfoMap;

	TMap<int, FItemAsset> itemAssetMap;

	//TMap<int, TWeakObjectPtr<AItemBase>> itemInFieldMap;

	UPROPERTY()
	TMap<FString, UItemObject*> itemObjectMap;

	UPROPERTY()
	TMap<FString, TWeakObjectPtr<AItemBase>> itemActorMap;

	//TMap<int, TWeakObjectPtr<UItemObject>> itemObjectPickedMap;

	TMap<int, FTemp> tempMap;

	TMap<int, FMemoryWriter> tempMap2;

};

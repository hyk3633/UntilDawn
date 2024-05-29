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

protected:

	virtual void BeginPlay() override;

	FItemAsset GetItemAssetMap(const int itemKey);

public:

	TWeakObjectPtr<UItemObject> GetPlayersPossessedItem(const PossessedItem& possessed);

	TWeakObjectPtr<AItemBase> GetPlayersEquippedItem(const EquippedItem& equipped);

	void SpawnItem(const FString& itemID, const int itemKey, const FVector location);

protected:

	TWeakObjectPtr<UItemObject> CreateItemObject(const FString& itemID, const int itemKey);

	UItemObject* NewItemObject(const EItemMainType itemType);

	TWeakObjectPtr<AItemBase> FindItemActor(TWeakObjectPtr<UItemObject> itemObj);

public:

	TWeakObjectPtr<UItemObject> GetItemObject(const FString& itemID, const int itemKey, const int itemQuantity);

	TWeakObjectPtr<UItemObject> GetItemObject(const FString& itemID);

	TWeakObjectPtr<AItemBase> GetItemActor(const FString& itemID, const int itemKey, const int itemQuantity);

	TWeakObjectPtr<AItemBase> GetItemActor(const FString& itemID);

	TWeakObjectPtr<AItemBase> GetItemActor(TWeakObjectPtr<UItemObject> itemObj);

	TWeakObjectPtr<AItemBase> GetItemActorInField(const FString& itemID);

	void ItemEquipped(const int playerNumber, const FString& itemID, TWeakObjectPtr<AItemBase> itemActor);

protected:

	void AddToPossessedItemsMap(const int playerNumber, TWeakObjectPtr<AItemBase> itemActor);

public:

	void ItemPickedUp(const FString& itemID);

protected:

	void AddToEquippedItemsMap(const int playerNumber, TWeakObjectPtr<UItemObject> itemObj);

public:

	// �ٸ� �÷��̾ ȹ���� ��� ������ ������Ʈ�� ���� �ʿ� �����ϰ� ���ʹ� Ǯ��
	void ItemPickedUpOtherPlayer(TWeakObjectPtr<APlayerCharacter> player, const FString& itemID);

	// �÷��̾ �ٸ� �÷��̾ �������� ���� ��� ������ ������Ʈ�� Ǯ���� ���Ϳ� �����Ͽ� ����
	TWeakObjectPtr<AItemBase> DropItem(TWeakObjectPtr<UItemObject> droppedItemObj);

protected:

	// �پ� �������� �ı�, ������ ������Ʈ�� �����ϰ� ���ʹ� Ǯ��
	void DestroyItem(const FString& itemID);

public:

	void OtherPlayerUseItem(TWeakObjectPtr<APlayerCharacter> player, const FString& itemID, const int consumedAmount);

	void RemovePlayersItems(const int playerNumber);

protected:

	UFUNCTION()
	void ItemExhausted(const FString& itemID);

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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Enums/ItemType.h"
#include "../Structs/ItemInfo.h"
#include "../Structs/ItemAsset.h"
#include "ItemManager.generated.h"

class UDataTable;
class UActorSpawner;
class UActorPooler;
class AItemBase;
class UItemObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UItemManager();

protected:

	virtual void BeginPlay() override;

	void Initialize();

	void ReadJson(const TArray<TSharedPtr<FJsonValue>>* jsonItems);

	void FillItemAssetMap();

	void GetData(const int itemKey, FItemInfo* newInfo);

	UFUNCTION()
	void ItemPicked(const int itemID);

public:

	void SpawnItem(const int itemID, const int itemKey, const FVector location);

	TWeakObjectPtr<UItemObject> GetItemObject(const int itemID);

	// �ٸ� �÷��̾ ȹ���� ��� ������ ������Ʈ�� ���� �ʿ� �����ϰ� ���ʹ� Ǯ��
	void ItemPickUpOtherPlayer(const int itemID);

	// �÷��̾ �ٸ� �÷��̾ �������� ���� ��� ������ ������Ʈ�� Ǯ���� ���Ϳ� �����Ͽ� ����
	TWeakObjectPtr<AItemBase> DropItem(TWeakObjectPtr<UItemObject> droppedItemObj);

	// �پ� �������� �ı�, ������ ������Ʈ�� �����ϰ� ���ʹ� Ǯ��
	void DestroyItem(const int itemID);

private:

	UPROPERTY(VisibleAnywhere, Category = "Item Info", meta = (AllowPrivateAccess = "true"))
	UDataTable* itemAssetDataTable;

	UPROPERTY()
	UActorSpawner* actorSpawner;

	UPROPERTY()
	TMap<int, UActorPooler*> itemPoolerMap;

	TMap<int, FItemInfo*> itemInfoMap;

	TMap<int, FItemAsset*> itemAssetMap;

	TMap<int, TWeakObjectPtr<AItemBase>> itemInFieldMap;

	UPROPERTY()
	TMap<int, UItemObject*> itemObjectMap;

	TMap<int, TWeakObjectPtr<UItemObject>> itemObjectPickedMap;

};

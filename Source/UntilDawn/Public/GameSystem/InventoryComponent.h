// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/Tile.h"
#include "../Enums/EquipmentBox.h"
#include "../Enums/ItemType.h"
#include "../Enums/PermanentItemType.h"
#include "InventoryComponent.generated.h"

class APlayerCharacter;
class UItemObject;
class AItemBase;
class AItemWeapon;

DECLARE_DELEGATE(DelegateOnInventoryChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	DelegateOnInventoryChanged DOnInventoryChanged;

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddItemAt(TWeakObjectPtr<UItemObject> newItemObj);

	void AddItemAt(TWeakObjectPtr<UItemObject> newItemObj, const FTile& topLeft);

	bool IsRoomAvailable(TWeakObjectPtr<UItemObject> newItemObj, const FTile& topLeft);

protected:

	TPair<bool, TWeakObjectPtr<UItemObject>> GetItemAtIndex(const int index);

	FTile IndexToTile(int index);

public:

	bool IsTileValid(FTile tile);

	int TileToIndex(FTile tile);

	void RemoveItem(TWeakObjectPtr<UItemObject> removedItem);

	void RemoveEquipmentItem(const int boxNumber, const EEquipmentBox boxType);

	TMap<TWeakObjectPtr<UItemObject>, FTile> GetAllItems() const;

	void EquipItem(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor);

	void UnequipItem(TWeakObjectPtr<AItemBase> itemActor);

	void Attack(TWeakObjectPtr<APlayerController> ownerController);

	EPermanentItemType ArmRecentWeapon();

	EPermanentItemType GetCurrentWeaponType() const;

	void DisarmWeapon();

	void InitializeEquippedWeaponArr(const int size);

	int GetSlotNumber(TWeakObjectPtr<UItemObject> itemObj);

	void SetCharacter(TWeakObjectPtr<APlayerCharacter> character);

	TWeakObjectPtr<UItemObject> GetItemObjectOfType(const EItemMainType itemType);

public:

	FORCEINLINE int GetColumns() const { return columns; }
	FORCEINLINE void SetColumns(int col) { columns = col; }
	FORCEINLINE int GetRows() const { return rows; }
	FORCEINLINE void SetRows(int row) { rows = row; }

private:

	TWeakObjectPtr<APlayerCharacter> ownerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int columns;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int rows;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	bool isDirty;

	// 그리드용 배열은 격자 형태로 저장하므로 단순 숫자 값만을 저장하도록 하고 아이템 오브젝트는 일차원 배열에 저장
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TMap<TWeakObjectPtr<UItemObject>, FTile> items;

	TArray<TWeakObjectPtr<UItemObject>> grids;

	TArray<TWeakObjectPtr<AItemBase>> equippedItems;

	TWeakObjectPtr<AItemBase> armedWeapon;
	
	int recentWeaponSlot = -1;

};

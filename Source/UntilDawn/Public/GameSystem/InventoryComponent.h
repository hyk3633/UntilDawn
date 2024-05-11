// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/Tile.h"
#include "../Enums/EquipmentBox.h"
#include "../Enums/ItemType.h"
#include "../Enums/WeaponType.h"
#include "InventoryComponent.generated.h"

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

	bool TryAddItem(TWeakObjectPtr<UItemObject> newItemObj);

	void AddItemAt(TWeakObjectPtr<UItemObject> newItemObj, int topLeftIndex);

	bool IsRoomAvailable(TWeakObjectPtr<UItemObject> newItemObj, int topLeftIndex);

protected:

	TPair<bool, TWeakObjectPtr<UItemObject>> GetItemAtIndex(const int index);

	FTile IndexToTile(int index);

public:

	bool IsTileValid(FTile tile);

	int TileToIndex(FTile tile);

	void RemoveItem(TWeakObjectPtr<UItemObject> removedItem);

	void RemoveEquipmentItem(const int boxNumber, const EEquipmentBox boxType);

	void GetAllItems(TMap<TWeakObjectPtr<UItemObject>, FTile>& itemsAll);

	void EquipItem(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor);

	void UnequipItem(TWeakObjectPtr<AItemBase> itemActor);

	void Attack(TWeakObjectPtr<APlayerController> ownerController);

	EWeaponType ArmRecentWeapon();

	EWeaponType GetCurrentWeaponType() const;

	void DisarmWeapon();

	void InitializeEquippedWeaponArr(const int size);

	int GetSlotNumber(TWeakObjectPtr<UItemObject> itemObj);

	FORCEINLINE int GetColumns() const { return columns; }
	FORCEINLINE void SetColumns(int col) { columns = col; }
	FORCEINLINE int GetRows() const { return rows; }
	FORCEINLINE void SetRows(int row) { rows = row; }

private:

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int columns;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int rows;

	// 그리드용 배열은 격자 형태로 저장하므로 단순 숫자 값만을 저장하도록 하고 아이템 오브젝트는 일차원 배열에 저장
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<UItemObject>> items;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	bool isDirty;

	TArray<TWeakObjectPtr<AItemBase>> equippedItems;

	TWeakObjectPtr<AItemBase> armedWeapon;
	
	int recentWeaponSlot = -1;

};

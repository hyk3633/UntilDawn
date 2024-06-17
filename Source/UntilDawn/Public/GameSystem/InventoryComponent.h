// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/Tile.h"
#include "../Enums/EquipmentSlot.h"
#include "../Enums/ItemType.h"
#include "../Enums/WeaponType.h"
#include "../Enums/InputType.h"
#include "../Enums/AmmoType.h"
#include "InventoryComponent.generated.h"

class APlayerCharacter;
class UItemObject;
class AItemBase;
class AItemWeapon;
class UGameplayAbility;
class UItemAmmo;

DECLARE_DELEGATE(DelegateOnInventoryChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTILDAWN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	DelegateOnInventoryChanged DOnInventoryChanged;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* 초기 설정 */

	void SetCharacter(TWeakObjectPtr<APlayerCharacter> character);

	void InitializeEquippedWeaponArr(const int size);

	void SetRowColumn(const uint8 r, const uint8 c);

	FORCEINLINE uint8 GetColumns() const { return columns; }
	FORCEINLINE uint8 GetRows() const { return rows; }

	/* 인벤토리 자료구조 질의 */

	// 아이템 배열에 아이템을 추가할 수 있는지 검사
	bool IsRoomAvailable(TWeakObjectPtr<UItemObject> newItemObj, const FTile& topLeft);

protected:

	// 해당 타일 위치가 올바른지 검사
	bool IsTileValid(FTile tile);

	TPair<bool, TWeakObjectPtr<UItemObject>> GetItemAtIndex(const int index);

public:

	TMap<TWeakObjectPtr<UItemObject>, FTile> GetAllItems() const;

	/* 아이템 인벤토리에 추가 */

	void AddItemAt(TWeakObjectPtr<UItemObject> newItemObj);

	void AddItemAt(TWeakObjectPtr<UItemObject> newItemObj, const FTile& topLeft);

protected:

	int TileToIndex(FTile tile);

public:

	/* 아이템 인벤토리에서 삭제 */

	void RemoveItem(TWeakObjectPtr<UItemObject> removedItem);

protected:

	void RemoveItemGrid(TWeakObjectPtr<UItemObject> removedItem, const FTile& topLeft);

public:

	void RemoveEquipmentItem(const int boxNumber, const EEquipmentSlot slotType);

	/* 아이템 장착 / 장착 해제 */

	void EquipItem(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor);

	void UnequipItem(TWeakObjectPtr<AItemBase> itemActor);

	TWeakObjectPtr<AItemBase> ArmRecentWeapon();

	void DisarmWeapon();

	TWeakObjectPtr<AItemBase> ChangeWeapon();

	bool IsAnyWeaponArmed();

	/* 아이템 사용 */

	void UsingConsumableItemOfType(const EItemMainType itemType);

	void Attack();

	bool IsWeaponUsable();

	TWeakObjectPtr<UItemAmmo> FindAmmo(const EAmmoType ammoType);

	/* Getter */

	EWeaponType GetCurrentWeaponType() const;

	int GetSlotNumber(TWeakObjectPtr<UItemObject> itemObj);

	TWeakObjectPtr<UItemObject> GetItemObjectOfType(const EItemMainType itemType);

private:

	TWeakObjectPtr<APlayerCharacter> ownerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	uint8 columns;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	uint8 rows;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	bool isDirty;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TMap<TWeakObjectPtr<UItemObject>, FTile> items;

	TArray<TWeakObjectPtr<UItemObject>> grids;

	TArray<TWeakObjectPtr<AItemBase>> equippedItems;

	TWeakObjectPtr<AItemBase> armedWeapon;
	
	int8 recentWeaponSlot = -1;

};

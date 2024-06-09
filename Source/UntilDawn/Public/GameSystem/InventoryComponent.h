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

protected:

	void RemoveItemGrid(TWeakObjectPtr<UItemObject> removedItem, const FTile& topLeft);

public:

	void RemoveEquipmentItem(const int boxNumber, const EEquipmentSlot slotType);

	TMap<TWeakObjectPtr<UItemObject>, FTile> GetAllItems() const;

	void EquipItem(const int boxNumber, TWeakObjectPtr<AItemBase> itemActor);

	void UnequipItem(TWeakObjectPtr<AItemBase> itemActor);

	void Attack();

	bool IsWeaponUsable();

	TWeakObjectPtr<AItemBase> ArmRecentWeapon();

public:

	EWeaponType GetCurrentWeaponType() const;

	void DisarmWeapon();

	void InitializeEquippedWeaponArr(const int size);

	int GetSlotNumber(TWeakObjectPtr<UItemObject> itemObj);

	void SetCharacter(TWeakObjectPtr<APlayerCharacter> character);

	TWeakObjectPtr<UItemObject> GetItemObjectOfType(const EItemMainType itemType);

	TWeakObjectPtr<UItemAmmo> FindAmmo(const EAmmoType ammoType);

	void UsingConsumableItemOfType(const EItemMainType itemType);

	bool IsAnyWeaponArmed();

	TWeakObjectPtr<AItemBase> ChangeWeapon();

public:

	FORCEINLINE uint8 GetColumns() const { return columns; }
	FORCEINLINE uint8 GetRows() const { return rows; }

	void SetRowColumn(const uint8 r, const uint8 c);

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

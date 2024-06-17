// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Structs/ItemAsset.h"
#include "../Structs/ItemInfo.h"
#include "../Structs/Tile.h"
#include "Serialization/BufferArchive.h"
#include "GameplayTagContainer.h"
#include "ItemObject.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(DelegateUpdateItemQuantity, uint8 quantity);

class AItemBase;
class APlayerControllerMainMap;
class APlayerCharacter;
class UGameplayAbility;

UCLASS(Abstract)
class UNTILDAWN_API UItemObject : public UObject
{
	GENERATED_BODY()

public:

	UItemObject();
	~UItemObject() = default;

	DelegateUpdateItemQuantity DUpdateItemQuantity;

	void Init(const FString& itemId, FItemInfo newInfo, TMap<FString, TSharedPtr<FJsonValue>> concreteInfoMap, FItemAsset newAsset);

protected:

	virtual void ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap) PURE_VIRTUAL(UItemObject::ParseItemConcreteInfo, );

	virtual void MakeItemFunction() PURE_VIRTUAL(UItemObject::MakeItemFunction, );

public:

	/* Getter Setter */

	void Rotate();

	void ResetOwner();

	/* 아이템 오너 */

	void SetOwnerController(TWeakObjectPtr<APlayerControllerMainMap> controller);

	FORCEINLINE TWeakObjectPtr<APlayerControllerMainMap> GetOwnerController() const { return ownerController; }

	void SetOwnerCharacter(TWeakObjectPtr<APlayerCharacter> character);

	FORCEINLINE TWeakObjectPtr<APlayerCharacter> GetOwnerCharacter() const { return ownerCharacter; }

	/* 아이템 정보 */

	FORCEINLINE FString GetItemID() const { return itemID; }

	FORCEINLINE FItemInfo GetItemInfo() const { return itemInfo; }

	FORCEINLINE EItemMainType GetItemType() const { return itemInfo.itemType; }

	FORCEINLINE bool GetIsConsumable() const { return itemInfo.isConsumable; }

	void SetItemQuantity(const uint8 quantity);

	FORCEINLINE uint16 GetItemQuantity() const { return itemInfo.quantity; }

	/* 아이템 에셋 */

	FORCEINLINE USkeletalMesh* GetSkeletalMesh() const { return itemAsset.skeletalMesh; }

	FORCEINLINE UStaticMesh* GetStaticMesh() const { return itemAsset.staticMesh; }

	FORCEINLINE FName GetSocketName() const { return itemAsset.socketName; }

	UMaterialInstance* GetIcon() const;

	UMaterialInstance* GetRotatedIcon() const;

	FORCEINLINE const FGameplayTagContainer& GetGameplayTags() { return itemAsset.gameplayTags; };

	TSubclassOf<UGameplayAbility> GetAbility(const EInputType inputType);

	/* 아이템 UI 정보 */

	FIntPoint GetDimensions() const;

	FIntPoint GetRotatedDimensions() const;

	FORCEINLINE bool IsRotated() const { return rotated; }

	void SetTopLeft(const FTile& newTopLeft);

	FORCEINLINE FTile GetTopLeft() const { return topLeft; }

private:

	TWeakObjectPtr<APlayerControllerMainMap> ownerController;

	TWeakObjectPtr<APlayerCharacter> ownerCharacter;

	FString itemID;

	FItemAsset itemAsset;

	FItemInfo itemInfo;

	bool rotated;

	FTile topLeft;
	
};

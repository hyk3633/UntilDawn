// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/PoolableActor.h"
#include "../Enums/ItemType.h"
#include "GameplayTagContainer.h"
#include "ItemBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UItemObject;

UCLASS()
class UNTILDAWN_API AItemBase : public AActor, public IPoolableActor
{
	GENERATED_BODY()
	
public:	

	AItemBase();

	/* Ǯ���� ���� �������̽� ���� �Լ� */

	virtual void ActivateActor() override;

	virtual void DeactivateActor() override;

	virtual bool IsActorActivated() override;

	/* Ȱ��ȭ */

	void ActivateFieldMode();

	void ActivateEquipMode(const EItemMainType itemType);

	/* Getter Setter */

	FString GetItemID() const;

	void SetItemObject(TWeakObjectPtr<UItemObject> newItemCore);

	FORCEINLINE TWeakObjectPtr<UItemObject> GetItemObject() const { return itemObj; }

	EItemMainType GetItemType();

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const { return skeletalMesh; }

	FORCEINLINE UStaticMeshComponent* GetStaticMesh() const { return staticMesh; }

	FName GetSocketName() const;

	const FGameplayTagContainer& GetGameplayTags();

	/* ����Ʈ ���μ��� */

	void RenderCustomDepthOn();

	void RenderCustomDepthOff();

private:

	UPROPERTY()
	USceneComponent* scene;

	TWeakObjectPtr<UItemObject> itemObj;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* skeletalMesh;

	bool isActive;

};

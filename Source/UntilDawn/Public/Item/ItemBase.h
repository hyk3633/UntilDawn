// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/PoolableActor.h"
#include "../Enums/ItemType.h"
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

	FORCEINLINE TWeakObjectPtr<UItemObject> GetItemObject() const { return itemObj; };

	FName GetSocketName() const;

	FString GetItemID() const;

	void SetItemObject(TWeakObjectPtr<UItemObject> newItemCore);

	virtual void ActivateActor() override;

	virtual void DeactivateActor() override;

	virtual bool IsActorActivated() override;

	void ActivateEquipMode();

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const { return skeletalMesh; }

	FORCEINLINE UStaticMeshComponent* GetStaticMesh() const { return staticMesh; }

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	USceneComponent* scene;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* skeletalMesh;

	bool isActive;

	TWeakObjectPtr<UItemObject> itemObj;

};

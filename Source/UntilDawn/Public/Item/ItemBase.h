// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/PoolableActor.h"
#include "../Structs/ItemInfo.h"
#include "ItemBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;

UCLASS()
class UNTILDAWN_API AItemBase : public AActor, public IPoolableActor
{
	GENERATED_BODY()
	
public:	

	AItemBase();

	FORCEINLINE int GetNumber() const { return number; }

	void SetNumber(const int num);

	void SetItemInfo(const FItemInfo& info);

	virtual void ActivateActor() override;

	virtual void DeactivateActor() override;

	virtual bool IsActorActivated() override;

protected:

	virtual void BeginPlay() override;

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const { return skeletalMesh; }

private:

	UPROPERTY(VisibleAnywhere, Category = "Item Info")
	int number;

	UPROPERTY(VisibleAnywhere, Category = "Item Info")
	EItemState state;

	UPROPERTY(VisibleAnywhere, Category = "Item Info")
	EItemMainType mainType;

	ItemSubType itemSubType;

	UPROPERTY()
	USceneComponent* scene;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* skeletalMesh;

	bool isActive;

};

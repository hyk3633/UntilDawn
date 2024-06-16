
#include "Item/ItemBase.h"
#include "Item/ItemObject.h"
#include "Structs/ItemAsset.h"
#include "UntilDawn/UntilDawn.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(scene);

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	staticMesh->SetupAttachment(RootComponent);
	staticMesh->SetCollisionObjectType(ECC_Item);
	staticMesh->SetCollisionProfileName(FName("DeactivatedItem"));
	staticMesh->SetWorldScale3D(FVector(3, 3, 3));

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	skeletalMesh->SetupAttachment(RootComponent);
	skeletalMesh->SetCollisionObjectType(ECC_Item);
	skeletalMesh->SetCollisionProfileName(FName("DeactivatedItem"));
}

void AItemBase::ActivateActor()
{
	isActive = true;
}

void AItemBase::DeactivateActor()
{
	skeletalMesh->SetVisibility(false);
	skeletalMesh->SetCollisionProfileName(FName("DeactivatedItem"));
	staticMesh->SetVisibility(false);
	staticMesh->SetCollisionProfileName(FName("DeactivatedItem"));
	itemObj.Reset();
	isActive = false;
}

bool AItemBase::IsActorActivated()
{
	return isActive;
}

void AItemBase::ActivateFieldMode()
{
	skeletalMesh->SetVisibility(true);
	skeletalMesh->SetCollisionProfileName(FName("ActivatedItem"));
	staticMesh->SetVisibility(true);
	staticMesh->SetCollisionProfileName(FName("ActivatedItem"));
}

void AItemBase::ActivateEquipMode(const EItemMainType itemType)
{
	if (itemType == EItemMainType::ArmourItem)
	{
		skeletalMesh->SetVisibility(false);
	}
	else
	{
		skeletalMesh->SetVisibility(true);
	}
	skeletalMesh->SetCollisionResponseToChannel(ECC_ItemTrace, ECR_Ignore);
	isActive = true;
}

EItemMainType AItemBase::GetItemType()
{
	if (itemObj.IsValid())
	{
		return itemObj->GetItemType();
	}
	return EItemMainType::MAX;
}

void AItemBase::RenderCustomDepthOn()
{
	skeletalMesh->SetRenderCustomDepth(true);
	staticMesh->SetRenderCustomDepth(true);
}

void AItemBase::RenderCustomDepthOff()
{
	skeletalMesh->SetRenderCustomDepth(false);
	staticMesh->SetRenderCustomDepth(false);
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FName AItemBase::GetSocketName() const
{
	return itemObj->GetSocketName();
}

FString AItemBase::GetItemID() const
{
	return itemObj->GetItemID();
}

const FGameplayTagContainer& AItemBase::GetGameplayTags()
{
	return itemObj->GetGameplayTags();
}

void AItemBase::SetItemObject(TWeakObjectPtr<UItemObject> newItemObj)
{
	itemObj = newItemObj;
	skeletalMesh->SetSkeletalMesh(newItemObj->GetSkeletalMesh());
	staticMesh->SetStaticMesh(newItemObj->GetStaticMesh());
}
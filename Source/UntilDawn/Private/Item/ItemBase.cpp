
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

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	skeletalMesh->SetupAttachment(RootComponent);
	skeletalMesh->SetCollisionObjectType(ECC_Item);
	skeletalMesh->SetCollisionProfileName(FName("DeactivatedItem"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Axe.SK_Axe'"));
	if (skeletalMeshAsset.Succeeded()) { skeletalMesh->SetSkeletalMesh(skeletalMeshAsset.Object); }
}

void AItemBase::ActivateActor()
{
	skeletalMesh->SetVisibility(true);
	skeletalMesh->SetCollisionProfileName(FName("ActivatedItem"));
	isActive = true;
}

void AItemBase::DeactivateActor()
{
	skeletalMesh->SetVisibility(false);
	skeletalMesh->SetCollisionProfileName(FName("DeactivatedItem"));
	itemObj.Reset();
	isActive = false;
}

bool AItemBase::IsActorActivated()
{
	return isActive;
}

void AItemBase::ActivateEquipMode()
{
	skeletalMesh->SetVisibility(true);
	skeletalMesh->SetCollisionResponseToChannel(ECC_ItemTrace, ECR_Ignore);
	isActive = true;
}

EItemMainType AItemBase::GetItemType()
{
	if (itemObj.IsValid())
	{
		return StaticCast<EItemMainType>(itemObj->GetItemType());
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

void AItemBase::SetItemObject(TWeakObjectPtr<UItemObject> newItemObj)
{
	itemObj = newItemObj;
	skeletalMesh->SetSkeletalMesh(newItemObj->GetSkeletalMesh());
	staticMesh->SetStaticMesh(newItemObj->GetStaticMesh());
}

#include "Item/ItemBase.h"
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
	isActive = false;
}

bool AItemBase::IsActorActivated()
{
	return isActive;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::SetNumber(const int num)
{
	number = num;
}

void AItemBase::SetItemInfo(const FItemInfo& info)
{
	state = info.state;
	if (state == EItemState::Activated)
	{
		ActivateActor();
	}
	else
	{
		DeactivateActor();
	}
	mainType = info.mainType;
	itemSubType = info.itemSubType;
	SetActorLocation(info.location);
}


#pragma once

#include "ItemInfo.h"
#include "Engine/DataTable.h"
#include "ItemAsset.generated.h"

USTRUCT(BlueprintType)
struct FItemAsset : public FTableRowBase
{
	GENERATED_BODY()

public:

	FItemAsset() : skeletalMesh(nullptr), staticMesh(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* skeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* iconRotated;

};
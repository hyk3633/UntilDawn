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

	void Copy(FItemAsset* asset)
	{
		skeletalMesh	= asset->skeletalMesh;
		staticMesh		= asset->staticMesh;
		icon			= asset->icon;
		iconRotated		= asset->iconRotated;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* skeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* iconRotated;

};
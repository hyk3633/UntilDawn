#pragma once

#include "Engine/DataTable.h"
#include "ZombieAsset.generated.h"

USTRUCT(BlueprintType)
struct FZombieAsset : public FTableRowBase
{
	GENERATED_BODY()

public:

	FZombieAsset() : skeletalMesh(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* skeletalMesh;

};
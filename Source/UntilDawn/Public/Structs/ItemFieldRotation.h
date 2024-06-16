#pragma once

#include "Engine/DataTable.h"
#include "ItemFieldRotation.generated.h"

USTRUCT(BlueprintType)
struct FItemFieldRotation : public FTableRowBase
{
	GENERATED_BODY()

public:

	FItemFieldRotation() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator rotation;

};
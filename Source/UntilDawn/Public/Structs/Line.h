#pragma once
#include "Line.generated.h"

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_BODY()

public:

	FLine() {}

	FLine(FVector2D s, FVector2D e) : start(s), end(e) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D end;

};
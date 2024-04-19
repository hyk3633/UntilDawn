#pragma once
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()

public:

	FTile() {}

	FTile(int x, int y) : X(x), Y(y) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y;

};
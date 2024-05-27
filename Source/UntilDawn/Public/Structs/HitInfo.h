#pragma once
#include <fstream>
#include "../../UntilDawn.h"
#include "HitInfo.generated.h"

using std::istream;
using std::ostream;

USTRUCT()
struct FHitInfo
{
	GENERATED_BODY()

public:

	FHitInfo() {}

	FHitInfo(int number, bool isP, FVector hitLoc, FRotator hitRot) :
		characterNumber(number), isPlayer(isP), hitLocation(hitLoc), hitRotation(hitRot) {}

	UPROPERTY()
	int characterNumber;

	UPROPERTY()
	bool isPlayer;

	UPROPERTY()
	FVector hitLocation;

	UPROPERTY()
	FRotator hitRotation;

	friend ostream& operator<<(ostream& stream, FHitInfo& hitInfo)
	{
		stream << hitInfo.characterNumber << "\n";
		stream << hitInfo.isPlayer << "\n";
		stream << hitInfo.hitLocation.X << "\n" << hitInfo.hitLocation.Y << "\n" << hitInfo.hitLocation.Z << "\n";
		stream << hitInfo.hitRotation.Pitch << "\n" << hitInfo.hitRotation.Yaw << "\n" << hitInfo.hitRotation.Roll << "\n";
		return stream;
	}

	friend istream& operator>>(istream& stream, FHitInfo& hitInfo)
	{
		stream >> hitInfo.characterNumber;
		stream >> hitInfo.isPlayer;
		stream >> hitInfo.hitLocation.X >> hitInfo.hitLocation.Y >> hitInfo.hitLocation.Z;
		stream >> hitInfo.hitRotation.Pitch >> hitInfo.hitRotation.Yaw >> hitInfo.hitRotation.Roll;
		return stream;
	}
};

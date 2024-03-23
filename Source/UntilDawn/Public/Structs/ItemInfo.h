#pragma once
#include "../Enums/ItemType.h"
#include <sstream>
#include "ItemInfo.generated.h"

union ItemSubType
{
	EMeleeWeaponType meleeWeaponType;
};

USTRUCT()
struct FItemInfo
{
	GENERATED_BODY()

public:

	EItemState state;

	EItemMainType mainType;

	ItemSubType itemSubType;

	FVector location;

	friend std::istream& operator>>(std::istream& stream, FItemInfo& item)
	{
		int itemEnum = 0;

		stream >> itemEnum;
		item.state = static_cast<EItemState>(itemEnum);

		stream >> itemEnum;
		item.mainType = static_cast<EItemMainType>(itemEnum);

		stream >> itemEnum;
		item.itemSubType.meleeWeaponType = static_cast<EMeleeWeaponType>(itemEnum);

		stream >> item.location.X >> item.location.Y >> item.location.Z;

		return stream;
	}

	//friend std::ostream& operator<<(std::ostream& stream, FItemInfo* item)
	//{
	//	stream << item->number << "\n";
	//	stream << static_cast<int>(item->state) << "\n";
	//	stream << static_cast<int>(item->mainType) << "\n";
	//	stream << static_cast<int>(item->itemSubType.meleeWeaponType) << "\n"; // 타입에 따라 다르게 넣도록
	//	stream << item->location;
	//}
};
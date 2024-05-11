#pragma once
#include <string>
#include <fstream>

struct EquippedItem
{
	std::string itemID;
	int itemKey;
	int slotNumber;
	friend std::istream& operator>>(std::istream& stream, EquippedItem& equipped)
	{
		stream >> equipped.itemID;
		stream >> equipped.itemKey;
		stream >> equipped.slotNumber;
		return stream;
	}
};
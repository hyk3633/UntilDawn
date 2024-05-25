#pragma once
#include <string>
#include <fstream>

struct EquippedItem
{
	std::string itemID;
	int itemKey;
	int slotNumber;
	bool isArmed;
	friend std::istream& operator>>(std::istream& stream, EquippedItem& equipped)
	{
		stream >> equipped.itemID;
		stream >> equipped.itemKey;
		stream >> equipped.slotNumber;
		stream >> equipped.isArmed;
		return stream;
	}
};
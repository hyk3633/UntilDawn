#pragma once
#include <string>
#include <fstream>
struct PossessedItem
{
	std::string itemID;
	int itemKey;
	int quantity;
	int isRotated;
	int topLeftX;
	int topLeftY;
	friend std::istream& operator>>(std::istream& stream, PossessedItem& possessed)
	{
		stream >> possessed.itemID;
		stream >> possessed.itemKey;
		stream >> possessed.quantity;
		stream >> possessed.isRotated;
		stream >> possessed.topLeftX;
		stream >> possessed.topLeftY;
		return stream;
	}
};
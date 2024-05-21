#pragma once
#include <fstream>

using std::istream;

struct PlayerStatus
{
	float health;

	friend istream& operator>>(istream& stream, PlayerStatus& status)
	{
		stream >> status.health;
		return stream;
	}
};
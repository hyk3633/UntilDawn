#pragma once

#include <sstream>
#include <vector>
#include <string>
#include "UntilDawn/UntilDawn.h"
#include "Enums/ZombieState.h"
#include "Structs/Pos.h"
#include "Structs/ItemInfo.h"
#include "Structs/EquippedItem.h"
#include "Structs/PlayerStatus.h"

enum class EZombieInfoBitType
{
	Location,
	Rotation,
	State,
	TargetNumber,
	NextLocation,
	MAX
};

typedef EZombieInfoBitType ZIBT;

struct ZombieInfo
{
	int recvInfoBitMask;

	FVector location;
	FRotator rotation;
	EZombieState state;
	int targetNumber;
	FVector nextLocation;

	friend std::istream& operator>>(std::istream& stream, ZombieInfo& info)
	{
		stream >> info.recvInfoBitMask;
		const int bitMax = static_cast<int>(ZIBT::MAX);
		for (int bit = 0; bit < bitMax; bit++)
		{
			if (info.recvInfoBitMask & (1 << bit))
			{
				info.ReceiveInfoToPacket(stream, bit);
			}
		}
		return stream;
	}

	void ReceiveInfoToPacket(std::istream& stream, const int bitType)
	{
		ZIBT type = static_cast<ZIBT>(bitType);
		switch (type)
		{
			case ZIBT::Location:
			{
				stream >> location.X >> location.Y >> location.Z;
				break;
			}
			case ZIBT::Rotation:
			{
				stream >> rotation.Pitch >> rotation.Yaw >> rotation.Roll;
				break;
			}
			case ZIBT::State:
			{
				int stateNumber = 0;
				stream >> stateNumber;
				state = static_cast<EZombieState>(stateNumber);
				break;
			}
			case ZIBT::TargetNumber:
			{
				stream >> targetNumber;
				break;
			}
			case ZIBT::NextLocation:
			{
				stream >> nextLocation.X >> nextLocation.Y >> nextLocation.Z;
				break;
			}
		}
	}
};

class ZombieInfoSet
{
public:

	ZombieInfoSet() = default;
	~ZombieInfoSet() = default;

	std::unordered_map<int, ZombieInfo> zombieInfoMap;

	friend std::istream& operator>>(std::istream& stream, ZombieInfoSet& info)
	{
		int size = 0, number = 0;
		info.zombieInfoMap.clear();

		stream >> size;
		for (int i = 0; i < size; i++)
		{
			stream >> number;
			stream >> info.zombieInfoMap[number];
		}
		return stream;
	}
};

struct CharacterInfo
{
	float vectorX, vectorY, vectorZ;
	float velocityX, velocityY, velocityZ;
	float pitch, yaw, roll;

	friend std::istream& operator>>(std::istream& stream, CharacterInfo& info)
	{
		stream >> info.vectorX >> info.vectorY >> info.vectorZ;
		stream >> info.pitch >> info.yaw >> info.roll;
		stream >> info.velocityX >> info.velocityY >> info.velocityZ;
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const CharacterInfo& info)
	{
		stream << info.vectorX << "\n" << info.vectorY << "\n" << info.vectorZ << "\n";
		stream << info.pitch << "\n" << info.yaw << "\n" << info.roll << "\n";
		stream << info.velocityX << "\n" << info.velocityY << "\n" << info.velocityZ << "\n";
		return stream;
	}
};

class PlayerInfoSet
{
public:

	PlayerInfoSet() = default;
	~PlayerInfoSet() = default;

	std::unordered_map<int, CharacterInfo> characterInfoMap;

	friend std::istream& operator>>(std::istream& stream, PlayerInfoSet& info)
	{
		int characterCount = 0;
		int characterNumber = 0;
		info.characterInfoMap.clear();

		stream >> characterCount;
		for (int i = 0; i < characterCount; i++)
		{
			stream >> characterNumber;
			stream >> info.characterInfoMap[characterNumber];
		}
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, PlayerInfoSet& info)
	{
		stream << info.characterInfoMap.size() << "\n";
		for (auto& p : info.characterInfoMap)
		{
			stream << p.first << "\n";
			stream << p.second << "\n";
		}
		return stream;
	}
};

struct PlayerInitialInfo
{
	std::string playerID;
	CharacterInfo characterInfo;
	PlayerStatus playerStatus;
	float pitch;
	std::vector<EquippedItem> equippedItems;

	friend std::istream& operator>>(std::istream& stream, PlayerInitialInfo& info)
	{
		stream >> info.playerID;
		stream >> info.characterInfo;
		stream >> info.pitch;
		stream >> info.playerStatus;
		int size = 0;
		stream >> size;
		info.equippedItems.reserve(size);
		for (int i = 0; i < size; i++)
		{
			EquippedItem equipped;
			stream >> equipped;
			info.equippedItems.push_back(equipped);
		}
		return stream;
	}
};

class PlayerInitialInfoSet
{
public:

	PlayerInitialInfoSet() = default;
	~PlayerInitialInfoSet() = default;

	std::unordered_map<int, PlayerInitialInfo> playerInitialInfoMap;

	void Deserialize(std::istream& stream)
	{
		int playerCount, playerNumber;
		stream >> playerCount;
		for (int i = 0; i < playerCount; i++)
		{
			stream >> playerNumber;
			stream >> playerInitialInfoMap[playerNumber];
		}
	}
};
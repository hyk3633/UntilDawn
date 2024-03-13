#pragma once

#include <sstream>
#include <vector>
#include "UntilDawn/UntilDawn.h"
#include "Enums/ZombieState.h"
#include "Structs/Pos.h"

struct ZombieInfo
{
	FVector location;
	FRotator rotation;
	EZombieState state;
	int targetNumber;
	FVector nextLocation;

	friend std::istream& operator>>(std::istream& stream, ZombieInfo& info)
	{
		stream >> info.location.X >> info.location.Y >> info.location.Z;
		stream >> info.rotation.Pitch >> info.rotation.Yaw >> info.rotation.Roll;
		int stateNumber = 0;
		stream >> stateNumber;
		info.state = static_cast<EZombieState>(stateNumber);
		stream >> info.targetNumber;
		stream >> info.nextLocation.X >> info.nextLocation.Y >> info.nextLocation.Z;
		return stream;
	}
};

class ZombieInfoSet
{
public:

	ZombieInfoSet() {};
	~ZombieInfoSet() {};

	std::unordered_map<int, ZombieInfo> zombieInfoMap;

	friend std::istream& operator>>(std::istream& stream, ZombieInfoSet& info)
	{
		int characterCount = 0;
		int characterNumber = 0;
		info.zombieInfoMap.clear();

		stream >> characterCount;
		for (int i = 0; i < characterCount; i++)
		{
			stream >> characterNumber;
			stream >> info.zombieInfoMap[characterNumber];
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
		stream >> info.velocityX >> info.velocityY >> info.velocityZ;
		stream >> info.pitch >> info.yaw >> info.roll;
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const CharacterInfo& info)
	{
		stream << info.vectorX << "\n" << info.vectorY << "\n" << info.vectorZ << "\n";
		stream << info.velocityX << "\n" << info.velocityY << "\n" << info.velocityZ << "\n";
		stream << info.pitch << "\n" << info.yaw << "\n" << info.roll << "\n";
		return stream;
	}
};

enum class EWrestleState
{
	ABLE,
	WRESTLING,
	WAITING
};

struct PlayerInfo
{
	CharacterInfo characterInfo;
	int infoBitMask;
	bool isZombiesSawMe;
	std::vector<int> zombiesWhoSawMe;
	bool isHitted;
	int zombieNumberAttackedMe;

	bool isWrestling = false;

	bool bSuccessToBlocking = false;

	int recvBitMask;

	EWrestleState wrestleState = EWrestleState::ABLE;

	friend std::ostream& operator<<(std::ostream& stream, const PlayerInfo& info)
	{
		stream << info.characterInfo;

		stream << info.infoBitMask << "\n";

		stream << info.isZombiesSawMe << "\n";
		if (info.isZombiesSawMe)
		{
			stream << info.zombiesWhoSawMe.size() << "\n";
			for (int n : info.zombiesWhoSawMe)
				stream << n << "\n";
		}
		if (info.infoBitMask & (1 << 2))
		{
			stream << info.isHitted << "\n";
			stream << info.zombieNumberAttackedMe << "\n";
		}
		if (info.infoBitMask & (1 << 3))
		{
			stream << info.bSuccessToBlocking << "\n";
		}
		return stream;
	}
};

class PlayerInfoSet
{
public:

	PlayerInfoSet() {};
	~PlayerInfoSet() {};

	std::unordered_map<int, PlayerInfo> characterInfoMap;

	friend std::istream& operator>>(std::istream& stream, PlayerInfoSet& info)
	{
		int characterCount = 0;
		int characterNumber = 0;
		info.characterInfoMap.clear();

		stream >> characterCount;
		for (int i = 0; i < characterCount; i++)
		{
			stream >> characterNumber;
			stream >> info.characterInfoMap[characterNumber].characterInfo;
			int bitmask;
			stream >> bitmask;
			info.characterInfoMap[characterNumber].recvBitMask = bitmask;
			if (bitmask & (1 << 3))
			{
				int wrestleState;
				stream >> wrestleState;
				info.characterInfoMap[characterNumber].wrestleState = static_cast<EWrestleState>(wrestleState);
			}
			if (bitmask & (1 << 4))
			{
				stream >> info.characterInfoMap[characterNumber].bSuccessToBlocking;
			}
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

class PlayerInfoSetEx : public PlayerInfoSet
{
public:

	PlayerInfoSetEx() {};
	~PlayerInfoSetEx() {};

	std::unordered_map<int, std::string> playerIDMap;

	void InputStreamWithID(std::istream& stream)
	{
		int playerCount = 0;
		std::string playerID = "";
		int playerNumber = 0;
		PlayerInfo playerInfo{};
		playerIDMap.clear();
		characterInfoMap.clear();
		stream >> playerCount;
		for (int i = 0; i < playerCount; i++)
		{
			stream >> playerNumber;
			stream >> playerID;
			stream >> playerInfo.characterInfo;
			playerIDMap[playerNumber] = playerID;
			characterInfoMap[playerNumber].characterInfo = playerInfo.characterInfo;
		}
	}
};
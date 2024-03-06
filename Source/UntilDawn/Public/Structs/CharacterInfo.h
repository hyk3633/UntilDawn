#pragma once

#include <sstream>
#include <vector>
#include "UntilDawn/UntilDawn.h"
#include "Enums/ZombieState.h"
#include "Structs/Pos.h"

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

struct PlayerInfo
{
	CharacterInfo characterInfo;
	bool isZombiesSawMe;
	std::vector<int> zombiesWhoSawMe;

	friend std::ostream& operator<<(std::ostream& stream, const PlayerInfo& info)
	{
		stream << info.characterInfo;
		stream << info.isZombiesSawMe << "\n";
		if (info.isZombiesSawMe)
		{
			stream << info.zombiesWhoSawMe.size() << "\n";
			for (int n : info.zombiesWhoSawMe)
				stream << n << "\n";
		}
		return stream;
	}
};

struct ZombieInfo
{
	CharacterInfo characterInfo;
	EZombieState state;
	bool bSetPath;
	std::vector<Pos> pathToTarget;
	int targetNumber;

	friend std::istream& operator>>(std::istream& stream, ZombieInfo& info)
	{
		int stateNumber = 0;
		stream >> info.characterInfo;
		stream >> stateNumber;
		info.state = static_cast<EZombieState>(stateNumber);
		stream >> info.targetNumber;
		stream >> info.bSetPath;
		if (info.bSetPath)
		{
			int size = 0;
			stream >> size;
			info.pathToTarget.clear();
			Pos pos;
			for (int i = 0; i < size; i++)
			{
				stream >> pos.x >> pos.y;
				info.pathToTarget.push_back(pos);
			}
		}
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, ZombieInfo& info)
	{
		stream << info.characterInfo;
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

	friend std::ostream& operator<<(std::ostream& stream, ZombieInfoSet& info)
	{
		stream << info.zombieInfoMap.size() << "\n";
		for (auto& p : info.zombieInfoMap)
		{
			stream << p.first << "\n";
			stream << p.second << "\n";
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
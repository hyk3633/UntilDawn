#pragma once

#include <sstream>
#include "UntilDawn/UntilDawn.h"
struct PlayerInfo
{
	float vectorX, vectorY, vectorZ;
	float velocityX, velocityY, velocityZ;
	float pitch, yaw, roll;

	friend std::istream& operator>>(std::istream& stream, PlayerInfo& info)
	{
		stream >> info.vectorX >> info.vectorY >> info.vectorZ;
		stream >> info.velocityX >> info.velocityY >> info.velocityZ;
		stream >> info.pitch >> info.yaw >> info.roll;
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const PlayerInfo& info)
	{
		stream << info.vectorX << "\n" << info.vectorY << "\n" << info.vectorZ << "\n";
		stream << info.velocityX << "\n" << info.velocityY << "\n" << info.velocityZ << "\n";
		stream << info.pitch << "\n" << info.yaw << "\n" << info.roll << "\n";
		return stream;
	}
};

class PlayerInfoSet
{
public:

	PlayerInfoSet() {};
	~PlayerInfoSet() {};

	std::unordered_map<int, PlayerInfo> playerInfoMap;

	friend std::istream& operator>>(std::istream& stream, PlayerInfoSet& info)
	{
		int playerCount = 0;
		int playerNumber = 0;
		PlayerInfo playerInfo{};

		stream >> playerCount;
		for (int i = 0; i < playerCount; i++)
		{
			stream >> playerNumber;
			stream >> playerInfo;
			info.playerInfoMap[playerNumber] = playerInfo;
		}
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const PlayerInfoSet& info)
	{
		stream << info.playerInfoMap.size() << "\n";
		for (auto& p : info.playerInfoMap)
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
		playerInfoMap.clear();
		stream >> playerCount;
		for (int i = 0; i < playerCount; i++)
		{
			stream >> playerNumber;
			stream >> playerID;
			stream >> playerInfo;
			playerIDMap[playerNumber] = playerID;
			playerInfoMap[playerNumber] = playerInfo;
		}
	}

	void OutputStreamWithID(std::ostream& stream)
	{
		stream << playerInfoMap.size() << "\n";		// 플레이어 수
		for (auto& p : playerInfoMap)
		{
			stream << p.first << "\n";				// 플레이어 번호
			stream << playerIDMap[p.first] << "\n";	// 플레이어 아이디
			stream << p.second << "\n";				// 플레이어 정보 구조체
		}
	}
};
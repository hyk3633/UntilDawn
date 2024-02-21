#pragma once

#include <sstream>
#include "UntilDawn/UntilDawn.h"

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

class CharacterInfoSet
{
public:

	CharacterInfoSet() {};
	~CharacterInfoSet() {};

	std::unordered_map<int, CharacterInfo> characterInfoMap;

	friend std::istream& operator>>(std::istream& stream, CharacterInfoSet& info)
	{
		int characterCount = 0;
		int characterNumber = 0;
		CharacterInfo characterInfo{};

		stream >> characterCount;
		for (int i = 0; i < characterCount; i++)
		{
			stream >> characterNumber;
			stream >> characterInfo;
			info.characterInfoMap[characterNumber] = characterInfo;
		}
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const CharacterInfoSet& info)
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

class PlayerInfoSetEx : public CharacterInfoSet
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
		CharacterInfo playerInfo{};
		playerIDMap.clear();
		characterInfoMap.clear();
		stream >> playerCount;
		for (int i = 0; i < playerCount; i++)
		{
			stream >> playerNumber;
			stream >> playerID;
			stream >> playerInfo;
			playerIDMap[playerNumber] = playerID;
			characterInfoMap[playerNumber] = playerInfo;
		}
	}

	void OutputStreamWithID(std::ostream& stream)
	{
		stream << characterInfoMap.size() << "\n";		// 플레이어 수
		for (auto& p : characterInfoMap)
		{
			stream << p.first << "\n";				// 플레이어 번호
			stream << playerIDMap[p.first] << "\n";	// 플레이어 아이디
			stream << p.second << "\n";				// 플레이어 정보 구조체
		}
	}
};
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

enum class EPlayerInfoBitTypeClient
{
	UncoveredByZombie,
	ZombieAttackResult,
	WrestlingResult,
	WrestlingEnd,
	MAX
};

typedef EPlayerInfoBitTypeClient PIBTC;

enum class EPlayerInfoBitTypeServer
{
	WrestlingState,
	PlayGrabReaction,
	MAX
};

typedef EPlayerInfoBitTypeServer PIBTS;

struct PlayerInfo
{
	// 필수 데이터
	CharacterInfo characterInfo;
	int sendInfoBitMask;

	// 서버 전송용 데이터
	std::vector<int> zombiesWhoSawMe;
	bool isHitted;
	int zombieNumberAttackedMe;
	bool isSuccessToBlocking = false;

	// 서버 수신용 데이터
	int recvInfoBitMask;
	EWrestleState wrestleState = EWrestleState::ABLE;
	bool isBlockingAction = false;

	friend std::ostream& operator<<(std::ostream& stream, const PlayerInfo& info)
	{
		stream << info.characterInfo;
		stream << info.sendInfoBitMask << "\n";
		const int bitMax = static_cast<int>(PIBTC::MAX);
		for (int bit = 0; bit < bitMax; bit++)
		{
			if (info.sendInfoBitMask & (1 << bit))
			{
				SaveInfoToPacket(stream, info, bit);
			}
		}
		return stream;
	}

	friend void SaveInfoToPacket(std::ostream& stream, const PlayerInfo& info, const int bitType)
	{
		PIBTC type = static_cast<PIBTC>(bitType);
		switch (type)
		{
			case PIBTC::UncoveredByZombie:
			{
				stream << info.zombiesWhoSawMe.size() << "\n";
				for (int n : info.zombiesWhoSawMe)
				{
					stream << n << "\n";
				}
				break;
			}
			case PIBTC::ZombieAttackResult:
			{
				stream << info.isHitted << "\n";
				stream << info.zombieNumberAttackedMe << "\n";
				break;
			}
			case PIBTC::WrestlingResult:
			{
				stream << info.isSuccessToBlocking << "\n";
				break;
			}
			case PIBTC::WrestlingEnd:
			{
				break;
			}
		}
	}

	friend std::istream& operator>>(std::istream& stream, PlayerInfo& info)
	{
		stream >> info.characterInfo;
		stream >> info.recvInfoBitMask;
		const int bitMax = static_cast<int>(PIBTS::MAX);
		for (int bit = 0; bit < bitMax; bit++)
		{
			if (info.recvInfoBitMask & (1 << bit))
			{
				ReceiveInfoToPacket(stream, info, bit);
			}
		}
		return stream;
	}

	friend void ReceiveInfoToPacket(std::istream& stream, PlayerInfo& info, const int bitType)
	{
		PIBTS type = static_cast<PIBTS>(bitType);
		switch (type)
		{
			case PIBTS::WrestlingState:
			{
				int wrestleState;
				stream >> wrestleState;
				info.wrestleState = static_cast<EWrestleState>(wrestleState);
				break;
			}
			case PIBTS::PlayGrabReaction:
			{
				stream >> info.isBlockingAction;
				break;
			}
		}
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
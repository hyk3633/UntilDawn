#pragma once

#include <sstream>
#include <vector>
#include "UntilDawn/UntilDawn.h"
#include "Enums/ZombieState.h"
#include "Structs/Pos.h"
#include "Structs/ItemInfo.h"

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

enum class EPlayerInfoBitTypeClient
{
	ZombiesInRange,
	ZombiesOutRange,
	ZombieAttackResult,
	PickedItems,
	MAX
};

typedef EPlayerInfoBitTypeClient PIBTC;

enum class EPlayerInfoBitTypeServer
{
	WrestlingState,
	MAX
};

typedef EPlayerInfoBitTypeServer PIBTS;

struct PlayerInfo
{
	// 필수 데이터
	CharacterInfo characterInfo;
	int sendInfoBitMask;

	// 서버 전송용 데이터
	std::vector<int> zombiesInRange, zombiesOutRange;	
	bool isHitted;						
	int zombieNumberAttackedMe;		

	// 서버 수신용 데이터
	int recvInfoBitMask;
	bool flag;

	friend std::ostream& operator<<(std::ostream& stream, const PlayerInfo& info)
	{
		stream << info.characterInfo;
		if (info.sendInfoBitMask == 0)
		{
			stream << false << "\n";
		}
		else
		{
			stream << true << "\n";
			stream << info.sendInfoBitMask << "\n";
			const int bitMax = static_cast<int>(PIBTC::MAX);
			for (int bit = 0; bit < bitMax; bit++)
			{
				if (info.sendInfoBitMask & (1 << bit))
				{
					SaveInfoToPacket(stream, info, bit);
				}
			}
		}
		return stream;
	}

	friend void SaveInfoToPacket(std::ostream& stream, const PlayerInfo& info, const int bitType)
	{
		PIBTC type = static_cast<PIBTC>(bitType);
		switch (type)
		{
			case PIBTC::ZombiesInRange:
			{
				stream << info.zombiesInRange.size() << "\n";
				for (int n : info.zombiesInRange)
				{
					stream << n << "\n";
				}
				break;
			}
			case PIBTC::ZombiesOutRange:
			{
				stream << info.zombiesOutRange.size() << "\n";
				for (int n : info.zombiesOutRange)
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
		}
	}

	friend std::istream& operator>>(std::istream& stream, PlayerInfo& info)
	{
		stream >> info.characterInfo;
		stream >> info.flag;

		if (info.flag)
		{
			info.recvInfoBitMask = 0;
			stream >> info.recvInfoBitMask;
			const int bitMax = static_cast<int>(PIBTS::MAX);
			for (int bit = 0; bit < bitMax; bit++)
			{
				if (info.recvInfoBitMask & (1 << bit))
				{
					ReceiveInfoToPacket(stream, info, bit);
				}
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
				break;
			}
		}
	}
};

class PlayerInfoSet
{
public:

	PlayerInfoSet() = default;
	~PlayerInfoSet() = default;

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

	PlayerInfoSetEx() = default;
	~PlayerInfoSetEx() = default;

	std::unordered_map<int, std::string> playerIDMap;

	void InputStreamWithID(std::istream& stream)
	{
		int playerCount = 0, playerNumber = 0;
		std::string playerID = "";
		PlayerInfo playerInfo{};

		playerIDMap.clear();
		characterInfoMap.clear();

		stream >> playerCount;
		for (int i = 0; i < playerCount; i++)
		{
			stream >> playerID;
			stream >> playerNumber;
			stream >> playerInfo.characterInfo;
			playerIDMap[playerNumber] = playerID;
			characterInfoMap[playerNumber].characterInfo = playerInfo.characterInfo;
		}
	}
};

class ItemInfoSet
{
public:

	ItemInfoSet() = default;
	~ItemInfoSet() = default;

	std::unordered_map<int, FItemInfo> itemInfoMap;

	friend std::istream& operator>>(std::istream& stream, ItemInfoSet& info)
	{
		info.itemInfoMap.clear();
		int size = 0, number = 0;
		stream >> size;
		FItemInfo itemInfo;
		while (size--)
		{
			stream >> number;
			stream >> info.itemInfoMap[number];
		}
		return stream;
	}
};
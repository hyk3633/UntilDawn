#pragma once

enum class EPacketType : uint8
{
	SIGNUP,
	LOGIN,
	SPAWNPLAYER,
	SYNCHPLAYER,
	PLAYERINPUTACTION,
	ZOMBIEINRANGE,
	ZOMBIEOUTRANGE,
	WRESTLINGRESULT,
	WRESTLINGSTART,
	SYNCHITEM,
	HITPLAYER,
	HITZOMBIE,
	ZOMBIEHITSME,
	PLAYERRESPAWN,
	PACKETTYPE_MAX, // MAX
	INITIALINFO,
	SYNCHZOMBIE,
	PLAYERDISCONNECTED,
	DESTROYITEM,
	PICKUPITEM,
	ZOMBIEDEAD,
	PLAYERDEAD,
};

// �α��� �ʿ��� �޴� ��Ŷ Ÿ�� �ִ� �� + 1
#define LOGINMAP_MAX static_cast<int>(EPacketType::LOGIN) + 1

// ���� �ʿ��� �޴� ��Ŷ Ÿ�� �ּ� ��
#define PACKETTYPE_MIN static_cast<int>(EPacketType::SPAWNPLAYER)

// ���� �ʿ��� �޴� ��Ŷ Ÿ�� �ִ� �� + 1
#define PACKETTYPE_MAX static_cast<int>(EPacketType::PLAYERDEAD) + 1
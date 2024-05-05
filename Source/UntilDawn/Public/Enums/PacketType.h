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
	ITEMTOPICKUP,
	ITEMGRIDPOINTUPDATE,
	ITEMTOEQUIP,
	ITEMTODROP,
	ATTACKRESULT,
	ZOMBIEHITSME,
	PLAYERRESPAWN,
	PACKETTYPE_MAX, // MAX
	INITIALINFO,
	SYNCHZOMBIE,
	PLAYERDISCONNECTED,
	ITEMPICKUPOTHER,
	ZOMBIEDEAD,
	PLAYERDEAD,
	SPAWNITEM,
};

// 로그인 맵에서 받는 패킷 타입 최대 값 + 1
#define LOGINMAP_MAX static_cast<int>(EPacketType::LOGIN) + 1
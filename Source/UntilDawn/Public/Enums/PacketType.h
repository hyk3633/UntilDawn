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
	PICKUP_ITEM,
	ITEMGRIDPOINTUPDATE,
	EQUIP_ITEM,
	DROP_ITEM,
	DROP_EQUIPPED_ITEM,
	UNEQUIP_ITEM,
	ATTACKRESULT,
	ZOMBIEHITSME,
	PLAYERRESPAWN,
	PROJECTILE,
	USINGITEM,
	DESTROYITEM,
	PACKETTYPE_MAX, // MAX
	INITIALINFO,
	SYNCHZOMBIE,
	PLAYERDISCONNECTED,
	ITEMPICKUPOTHER,
	ZOMBIEDEAD,
	PLAYERDEAD,
	SPAWNITEM,
	PLAYERINVENTORY,
	PLAYEREQUIPMENT,
	PLAYERSTATUS,
};

// 로그인 맵에서 받는 패킷 타입 최대 값 + 1
#define LOGINMAP_MAX static_cast<int>(EPacketType::LOGIN) + 1
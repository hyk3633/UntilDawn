#pragma once

enum class EPacketType : uint8
{
	SIGNUP,
	LOGIN,
	SPAWNPLAYER,
	SYNCHPLAYER,
	PLAYERINPUTACTION,
	WRESTLINGRESULT,
	WRESTLINGSTART,
	SYNCHITEM,
	HITPLAYER,
	HITZOMBIE,
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
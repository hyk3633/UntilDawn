#pragma once

enum class EPacketType : uint8
{
	SIGNUP,
	LOGIN,
	SPAWNPLAYER,
	SYNCH,
	PACKETTYPE_MAX,
	PLAYERDISCONNECTED
};
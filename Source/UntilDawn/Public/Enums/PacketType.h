#pragma once

enum class EPacketType : uint8
{
	// 로그인
	SIGNUP,
	LOGIN,

	// 월드 정보 초기화
	WORLD_INITIAL_INFO,

	// 플레이어 정보 및 상태 동기화
	SPAWN_PLAYER,
	PLAYER_INITIAL_INFO,
	PLAYER_INVENTORY,
	PLAYER_EQUIPMENT,
	SYNCH_PLAYER,
	HEALTH_CHANGED,
	PLAYER_DEAD,
	PLAYER_RESPAWN,
	PLAYER_DISCONNECTED,

	// 플레이어 좀비 레슬링
	WRESTLING_START,
	WRESTLING_RESULT,
	WRESTLING_CANCELED,

	// 플레이어 Sphere Collision에 좀비가 오버랩 되었을 경우 (좀비의 플레이어 감지)
	ZOMBIE_IN_RANGE,
	ZOMBIE_OUT_RANGE,
	
	// 좀비의 플레이어 공격  
	ZOMBIE_HITS_ME,

	// 플레이어의 인벤토리의 아이템 그리드 위치 변경
	ITEM_GRID_POINT_UPDATE,

	// 플레이어의 공격 동기화
	ACTIVATE_WEAPON_ABILITY,
	ATTACK_RESULT,
	KICKED_CHARACTERS,
	PROJECTILE,

	// 아이템 동기화
	SPAWN_ITEM,
	DESTROY_ITEM,
	PICKUP_ITEM,
	DROP_ITEM,
	EQUIP_ITEM,
	UNEQUIP_ITEM,
	DROP_EQUIPPED_ITEM,
	ARM_WEAPON,
	DISARM_WEAPON,
	CHANGE_WEAPON,
	USING_ITEM,

	// 좀비 동기화
	SYNCH_ZOMBIE,
	ZOMBIE_DEAD,
};

// 로그인 맵에서 받는 패킷 타입 최대 값 + 1
#define LOGINMAP_MAX static_cast<int>(EPacketType::LOGIN) + 1
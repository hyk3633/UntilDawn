#pragma once

enum class EPacketType : uint8
{
	// �α���
	SIGNUP,
	LOGIN,

	// ���� ���� �ʱ�ȭ
	WORLD_INITIAL_INFO,

	// �÷��̾� ���� �� ���� ����ȭ
	SPAWN_PLAYER,
	PLAYER_INITIAL_INFO,
	PLAYER_INVENTORY,
	PLAYER_EQUIPMENT,
	SYNCH_PLAYER,
	HEALTH_CHANGED,
	PLAYER_DEAD,
	PLAYER_RESPAWN,
	PLAYER_DISCONNECTED,

	// �÷��̾� ���� ������
	WRESTLING_START,
	WRESTLING_RESULT,
	WRESTLING_CANCELED,

	// �÷��̾� Sphere Collision�� ���� ������ �Ǿ��� ��� (������ �÷��̾� ����)
	ZOMBIE_IN_RANGE,
	ZOMBIE_OUT_RANGE,
	
	// ������ �÷��̾� ����  
	ZOMBIE_HITS_ME,

	// �÷��̾��� �κ��丮�� ������ �׸��� ��ġ ����
	ITEM_GRID_POINT_UPDATE,

	// �÷��̾��� ���� ����ȭ
	ACTIVATE_WEAPON_ABILITY,
	ATTACK_RESULT,
	KICKED_CHARACTERS,
	PROJECTILE,

	// ������ ����ȭ
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

	// ���� ����ȭ
	SYNCH_ZOMBIE,
	ZOMBIE_DEAD,
};

// �α��� �ʿ��� �޴� ��Ŷ Ÿ�� �ִ� �� + 1
#define LOGINMAP_MAX static_cast<int>(EPacketType::LOGIN) + 1
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include "Enums/PacketType.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "UntilDawn/UntilDawn.h"
#include "Structs/CharacterInfo.h"
#include "Structs/HitInfo.h"
#include "../Enums/InputType.h"
#include "CoreMinimal.h"

/**
 * 
 */

class UNTILDAWN_API ClientSocket : public FRunnable
{
public:

	ClientSocket();
	
	~ClientSocket();

	bool InitSocket();

	void StartSocket();

	/* 로그인 레벨 */

	void SendAccountInfo(const FText& id, const FText& pw, const bool isLogin);

	/* 메인 레벨 */

	void NotifyAccessingGame(const CharacterInfo& info);

	/* 캐릭터 동기화 */

	void SynchronizeMyCharacterInfo(const CharacterInfo& info, const float pitch);

	/* 좀비 상호작용 */

	void SendInRangeZombie(int zombieNumber);

	void SendOutRangeZombie(int zombieNumber);

	void SendZombieHitsMe(const int zombieNumber, const bool bResult, FHitInfo& hitInfo);

	void SendPlayerBlockingResult(const bool isSuccessToBlocking);

	/* 아이템 동기화 */

	void SendPickedItemInfo(const FString itemID);

	void UpdateItemGridPoint(const FString itemID, const int xPoint, const int yPoint, const bool isRotated);

	void SendItemInfoToEquip(const FString itemID, const int boxNumber);

	void DropEquippedItem(const FString itemID);

	void DropInventoryItem(const FString itemID);

	void UnequipItem(const FString itemID, const int xPoint, const int yPoint);

	void SendItemUsing(const FString& itemID, const int consumedAmount);

	void ArmWeapon(const FString& itemID);

	void DisarmWeapon();

	void ChangeWeapon(const FString& changedWeaponID);

	/* 공격 동기화 */

	void SendActivateWeaponAbility(const int32 inputType);

	void SendHittedCharacter(FHitInfo& hittedCharacter, const FString& itemID);

	void SendKickedCharacters(const int characterNumber, const bool isPlayer);

	void ReplicateProjectile(const FVector& location, const FRotator& rotation);

	/* 리스폰 */

	void SendRespawnRequest();

	/* 소켓 통신 */

	void Send(std::stringstream& sendStream);

	// FRunnable 가상 함수

	virtual bool Init() override;

	virtual uint32 Run() override;

public:

	virtual void Exit() override;

	virtual void Stop() override;

private:

	bool bRun;

	SOCKET clientSocket;

	HANDLE sendThread, recvThread;

	FRunnableThread* thread;

	char recvBuf[PACKET_SIZE];

public:

	TQueue<std::stringstream> messageQ;

};

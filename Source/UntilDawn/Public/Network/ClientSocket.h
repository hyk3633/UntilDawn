// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <sstream>
#include "Enums/PacketType.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "UntilDawn/UntilDawn.h"
#include "Structs/CharacterInfo.h"
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

	void SendAccountInfo(const FText& id, const FText& pw, const bool isLogin);

	void NotifyAccessingGame(const CharacterInfo& info);

	void SynchronizeMyCharacterInfo(const PlayerInfo& info);

	void SendPlayerInputAction(const int inputType);

	void SendInRangeZombie(int zombieNumber);

	void SendOutRangeZombie(int zombieNumber);

	void SendZombieHitsMe(int zombieNumber, bool bResult);

	void SendPlayerBlockingResult(const bool isSuccessToBlocking);

	void SendPickedItemInfo(const int itemID);

	void UpdateItemGridPoint(const int itemID, const int xPoint, const int yPoint, const bool isRotated);

	void SendItemInfoToEquip(const int itemID, const int boxNumber);

	void SendItemInfoToDrop(const int itemID);

	void SendHittedCharactersInfo(TArray<TPair<int, bool>>& hittedCharacters);

	void SendRespawnRequest();

	// FRunnable 가상 함수

	virtual bool Init() override;

	virtual uint32 Run() override;

public:

	virtual void Exit() override;

	virtual void Stop() override;

private:

	bool isInitialized;

	bool bRun;

	SOCKET clientSocket;

	HANDLE sendThread, recvThread;

	FRunnableThread* thread;

	char recvBuf[PACKET_SIZE];

	PlayerInfoSetEx newPlayerInfoSetEx;

	PlayerInfoSet synchPlayerInfoSet;

	ZombieInfoSet synchZombieInfoSet;

	ItemInfoSet synchItemInfoSet;

public:

	TQueue<std::stringstream> messageQ;

};

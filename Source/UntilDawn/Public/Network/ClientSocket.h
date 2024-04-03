// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <sstream>
#include <concurrent_queue.h>
#include "Enums/PacketType.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "UntilDawn/UntilDawn.h"
#include "Structs/CharacterInfo.h"
#include "CoreMinimal.h"

/**
 * 
 */

class APlayerControllerLoginMap;
class AGameModeMainMap;

class UNTILDAWN_API ClientSocket : public FRunnable
{
public:

	ClientSocket();
	
	~ClientSocket();

	bool InitSocket();

	void StartSocket();

	void Recv(std::stringstream&);

	void SendAccountInfo(const FText& id, const FText& pw, const bool isLogin);

	void NotifyAccessingGame(const CharacterInfo& info);

	void SynchronizeMyCharacterInfo(const PlayerInfo& info);

	void SendPlayerInputAction(const int inputType);

	void SendPlayerBlockingResult(const bool isSuccessToBlocking);

	void SendPickedItemInfo(const int itemNumber);

	void SendHitPlayerInfo(const int playerNumber);

	void SendHitZombieInfo(const int zombieNumber);

	void SendRespawnRequest();

	// FRunnable 가상 함수

	virtual bool Init() override;

	virtual uint32 Run() override;

protected:

	void ProcessPacket(const EPacketType type, std::stringstream& recvStream);

public:

	virtual void Exit() override;

	virtual void Stop() override;

	void SetPlayerController(APlayerControllerLoginMap* controller);

	void SetGameMode(AGameModeMainMap* gameMode);

	bool IsMessageQueueEmpty();

	void GetDataInMessageQueue(std::stringstream& recvStream);

private:

	bool isInitialized;

	bool bRun;

	SOCKET clientSocket;

	HANDLE sendThread, recvThread;

	FRunnableThread* thread;
	FRunnableThread* thread2;

	char recvBuf[PACKET_SIZE];

	TWeakObjectPtr<APlayerControllerLoginMap> ownerController;

	PlayerInfoSetEx newPlayerInfoSetEx;

	PlayerInfoSet synchPlayerInfoSet;

	ZombieInfoSet synchZombieInfoSet;

	ItemInfoSet synchItemInfoSet;

	TWeakObjectPtr<AGameModeMainMap> ownerGameMode;

	FCriticalSection criticalSection;
public:
	Concurrency::concurrent_queue<std::vector<char>> messageQ;
	std::vector<char> mVec;
};

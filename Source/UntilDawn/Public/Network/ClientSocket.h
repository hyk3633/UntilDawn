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

class APlayerControllerLoginMap;
class AGameModeMainMap;

class UNTILDAWN_API ClientSocket : public FRunnable
{
public:

	ClientSocket();
	
	~ClientSocket();

	bool InitSocket();

	void StartSocket();

	void StopThread();

	void Recv(std::stringstream&);

	void SendAccountInfo(const FText& id, const FText& pw, const bool isLogin);

	void NotifyAccessingGame(const CharacterInfo& info);

	void SynchronizeMyCharacterInfo(const PlayerInfo& info);

	void SendPlayerInputAction(const int inputType);

	void Send(std::stringstream&);

	// FRunnable 가상 함수

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Exit() override;

	virtual void Stop() override;

	FORCEINLINE void SetPlayerController(APlayerControllerLoginMap* controller) { ownerController = controller; }

	FORCEINLINE void SetGameMode(AGameModeMainMap* gameMode) { ownerGameMode = gameMode; }

private:

	SOCKET clientSocket;

	HANDLE sendThread, recvThread;

	FRunnableThread* thread;

	char recvBuf[PACKET_SIZE];

	UPROPERTY()
	APlayerControllerLoginMap* ownerController;

	PlayerInfoSetEx newPlayerInfoSetEx;

	PlayerInfoSet synchPlayerInfoSet;

	ZombieInfoSet synchZombieInfoSet;

	UPROPERTY()
	AGameModeMainMap* ownerGameMode;
};

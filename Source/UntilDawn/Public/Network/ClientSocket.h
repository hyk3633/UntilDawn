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

	void SynchronizeMyCharacterInfo(const CharacterInfo& info);

	void SendPlayerInputAction(const int inputType, const int weaponType);

	void SendInRangeZombie(int zombieNumber);

	void SendOutRangeZombie(int zombieNumber);

	void SendZombieHitsMe(int zombieNumber, bool bResult);

	void SendPlayerBlockingResult(const bool isSuccessToBlocking);

	void SendPickedItemInfo(const FString itemID);

	void UpdateItemGridPoint(const FString itemID, const int xPoint, const int yPoint, const bool isRotated);

	void SendItemInfoToEquip(const FString itemID, const int boxNumber);

	void DropEquippedItem(const FString itemID);

	void UnequipItem(const FString itemID, const int xPoint, const int yPoint);

	void SendItemInfoToDrop(const FString itemID);

	void SendHittedCharacters(TArray<TPair<int, bool>>& hittedCharacters);

	void SendRespawnRequest();

	void ReplicateProjectile(const FVector& location, const FRotator& rotation);

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

public:

	TQueue<std::stringstream> messageQ;

};

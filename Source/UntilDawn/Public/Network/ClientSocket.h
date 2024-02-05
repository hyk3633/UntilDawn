// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <sstream>
#include "Runtime/Core/Public/HAL/Runnable.h"
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

	void StopThread();

	static ClientSocket* GetSingleton()
	{
		static ClientSocket instance;
		return &instance;
	}

	// FRunnable 가상 함수

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Exit() override;

	virtual void Stop() override;

private:

	SOCKET clientSocket;

	HANDLE SendThread, RecvThread;

	FRunnableThread* Thread;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <sstream>
#include "Enums/PacketType.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "UntilDawn/UntilDawn.h"
#include "CoreMinimal.h"

/**
 * 
 */
class UNTILDAWN_API SendThread : public FRunnable
{
public:

	SendThread();

	~SendThread();

	// FRunnable 가상 함수



};

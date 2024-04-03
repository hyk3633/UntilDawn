// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <sstream>
#include "CoreMinimal.h"

/**
 * 
 */
class UNTILDAWN_API RecvThread : public FRunnable
{
public:
	RecvThread();
	~RecvThread();
};

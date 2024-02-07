// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/UntilDawnGameInstance.h"
#include "Network/ClientSocket.h"

ClientSocket* UUntilDawnGameInstance::GetSocket()
{
	static ClientSocket instance;
	return &instance;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/UntilDawnGameInstance.h"
#include "Network/ClientSocket.h"

ClientSocket* UUntilDawnGameInstance::GetSocket()
{
	if (socket == nullptr)
	{
		socket = new ClientSocket();
	}
	return socket;
}

void UUntilDawnGameInstance::SetPlayerIDAndNumber(const FString& id, const int number)
{
	playerID = id;
	playerNumber = number;
}

void UUntilDawnGameInstance::Shutdown()
{
	Super::Shutdown();

	if (socket)
	{
		socket->Stop();
		delete socket;
		socket = nullptr;
	}
}

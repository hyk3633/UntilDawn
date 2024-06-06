// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeLoginMap.h"
#include "Network/ClientSocket.h"
#include "Player/Login/PlayerControllerLoginMap.h"
#include "UI/Login/HUDLoginMap.h"
#include "UntilDawn/UntilDawn.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Kismet/GamePlayStatics.h"

AGameModeLoginMap::AGameModeLoginMap()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = nullptr;
	PlayerControllerClass = APlayerControllerLoginMap::StaticClass();
	HUDClass = AHUDLoginMap::StaticClass();
}

void AGameModeLoginMap::BeginPlay()
{
	Super::BeginPlay();

	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	if (clientSocket->InitSocket())
	{
		clientSocket->StartSocket();
		isConnected = true;
	}

	packetCallbacks = std::vector<void (AGameModeLoginMap::*)(std::stringstream&)>(LOGINMAP_MAX);
	packetCallbacks[static_cast<int>(EPacketType::SIGNUP)] = &AGameModeLoginMap::ReceiveSignUpResult;
	packetCallbacks[static_cast<int>(EPacketType::LOGIN)] = &AGameModeLoginMap::ReceiveLoginResult;

	loginController = Cast<APlayerControllerLoginMap>(UGameplayStatics::GetPlayerController(this, 0));
	loginController->SetSocket(clientSocket);
}

void AGameModeLoginMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	ProcessPacket();
}

void AGameModeLoginMap::ProcessPacket()
{
	if (clientSocket == nullptr)
		return;

	if (clientSocket->messageQ.IsEmpty())
		return;

	std::stringstream recvStream;
	clientSocket->messageQ.Dequeue(recvStream);

	int packetType = -1;
	recvStream >> packetType;
	if (packetType >= 0 && packetType < LOGINMAP_MAX)
	{
		if (packetCallbacks[packetType] == nullptr)
		{
			PLOG(TEXT("Callback is nullptr! : type number %d"), packetType);
		}
		else
		{
			(this->*packetCallbacks[packetType])(recvStream);
		}
	}
	else
	{
		PLOG(TEXT("Invalid packet number! : type number %d"), packetType);
	}
}

void AGameModeLoginMap::ReceiveSignUpResult(std::stringstream& recvStream)
{
	bool isGranted;
	recvStream >> isGranted;
	loginController->SetSignUpMessageText(isGranted);
}

void AGameModeLoginMap::ReceiveLoginResult(std::stringstream& recvStream)
{
	bool isGranted;
	recvStream >> isGranted;
	if (isGranted)
	{
		std::string playerID;
		int playerNumber = -1;
		recvStream >> playerID >> playerNumber;

		GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->SetPlayerIDAndNumber(FString(UTF8_TO_TCHAR(playerID.c_str())), playerNumber);
		loginController->SetLoginMessageTextAndLoginToMap(isGranted, FString(UTF8_TO_TCHAR(playerID.c_str())), playerNumber);
	}
	else
	{
		loginController->SetLoginMessageTextAndLoginToMap(isGranted);
	}
}

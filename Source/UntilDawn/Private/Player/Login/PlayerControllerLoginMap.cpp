// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Login/PlayerControllerLoginMap.h"
#include "Network/ClientSocket.h"
#include "UI/Login/HUDLoginMap.h"
#include "UntilDawn/UntilDawn.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Kismet/GamePlayStatics.h"

APlayerControllerLoginMap::APlayerControllerLoginMap()
{
	bShowMouseCursor = true;
}

void APlayerControllerLoginMap::BeginPlay()
{
	Super::BeginPlay();

	clientSocket = GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->GetSocket();
	clientSocket->SetPlayerController(this);
	if (clientSocket->InitSocket())
	{
		clientSocket->StartSocket();
		isConnected = true;
	}

	loginMapHUD = Cast<AHUDLoginMap>(GetHUD());
	if (loginMapHUD)
	{
		loginMapHUD->DSendAccountInfo.BindUFunction(this, FName("ReceiveAccountInfo"));
	}

	FInputModeUIOnly uiInputMode;
	SetInputMode(uiInputMode);

	packetCallbacks = std::vector<void (APlayerControllerLoginMap::*)(std::stringstream&)>(LOGINMAP_MAX);
	packetCallbacks[static_cast<int>(EPacketType::SIGNUP)]	= &APlayerControllerLoginMap::SetSignUpMessageText;
	packetCallbacks[static_cast<int>(EPacketType::LOGIN)]	= &APlayerControllerLoginMap::SetLoginMessageTextAndLoginToMap;
}

void APlayerControllerLoginMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!messageQ.empty())
	{
		ProcessPacket();
	}
}

void APlayerControllerLoginMap::ReceivePacket(std::stringstream& recvStream)
{
	FScopeLock Lock(&criticalSection);
	messageQ.push(std::move(recvStream));
}

void APlayerControllerLoginMap::ProcessPacket()
{
	FScopeLock Lock(&criticalSection);
	std::stringstream recvStream = std::move(messageQ.front());
	messageQ.pop();

	int packetType = -1;
	recvStream >> packetType;
	if (packetType >= 0 && packetType < LOGINMAP_MAX)
	{
		if (packetCallbacks[packetType] == nullptr)
		{
			ELOG(TEXT("Callback is nullptr!"));
		}
		else
		{
			(this->*packetCallbacks[packetType])(recvStream);
		}
	}
	else
	{
		ELOG(TEXT("Invalid packet number!"));
	}
}

void APlayerControllerLoginMap::ReceiveAccountInfo(const FText& id, const FText& pw, const bool isLogin)
{
	if (isConnected)
	{
		clientSocket->SendAccountInfo(id, pw, isLogin);
	}
}

void APlayerControllerLoginMap::SetSignUpMessageText(std::stringstream& recvStream)
{
	bool isGranted;
	recvStream >> isGranted;
	loginMapHUD->SetSignUpMessageBox(isGranted);
}

void APlayerControllerLoginMap::SetLoginMessageTextAndLoginToMap(std::stringstream& recvStream)
{
	bool isGranted;
	recvStream >> isGranted;
	loginMapHUD->SetLoginMessageBox(isGranted);
	if (isGranted)
	{
		int playerNumber = -1;
		recvStream >> playerNumber;
		double start = 0;
		recvStream >> start;
		double end = FPlatformTime::Seconds();
		PLOG(TEXT("elapsed login %f"), end - start);
		check(playerNumber >= 0);
		GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->SetPlayerNumber(playerNumber);
		GetWorldTimerManager().SetTimer(levelTransitionTimer, this, &APlayerControllerLoginMap::StartLevelTransition, 2.f, false);
	}
}

void APlayerControllerLoginMap::StartLevelTransition()
{
	UGameplayStatics::OpenLevel(this, TEXT("MainMap"));
}


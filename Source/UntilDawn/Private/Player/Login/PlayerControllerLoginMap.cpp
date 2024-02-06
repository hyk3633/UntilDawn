// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Login/PlayerControllerLoginMap.h"
#include "Network/ClientSocket.h"
#include "UI/Login/HUDLoginMap.h"
#include "UntilDawn/UntilDawn.h"

APlayerControllerLoginMap::APlayerControllerLoginMap()
{
	bShowMouseCursor = true;
}

void APlayerControllerLoginMap::BeginPlay()
{
	Super::BeginPlay();

	clientSocket = ClientSocket::GetSingleton();
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
}

void APlayerControllerLoginMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSetSignUpMessageText)
		SetSignUpMessageText();
	if (bSetLoginMessageText)
		SetLoginMessageText();
}

void APlayerControllerLoginMap::ReceiveSignUpRequestResult(const bool isGranted)
{
	bFlag = isGranted;
	bSetSignUpMessageText = true;
}

void APlayerControllerLoginMap::ReceiveLoginRequestResult(const bool isGranted)
{
	bFlag = isGranted;
	bSetLoginMessageText = true;
	PLOG(TEXT("recv %d"), isGranted);
}

void APlayerControllerLoginMap::ReceiveAccountInfo(const FText& id, const FText& pw, const bool isLogin)
{
	if(isConnected) clientSocket->SendAccountInfo(id, pw, isLogin);
}

void APlayerControllerLoginMap::SetSignUpMessageText()
{
	loginMapHUD->SetSignUpMessageBox(bFlag);
	bSetSignUpMessageText = false;
}

void APlayerControllerLoginMap::SetLoginMessageText()
{
	loginMapHUD->SetLoginMessageBox(bFlag);
	bSetLoginMessageText = false;
}


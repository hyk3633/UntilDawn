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

void APlayerControllerLoginMap::ReceiveLoginRequestResult(const bool isGranted, int number)
{
	bFlag = isGranted;
	bSetLoginMessageText = true;
	GetWorld()->GetGameInstance<UUntilDawnGameInstance>()->SetPlayerNumber(number);
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
	if(bFlag)
		GetWorldTimerManager().SetTimer(levelTransitionTimer, this, &APlayerControllerLoginMap::StartLevelTransition, 2.f, false);
}

void APlayerControllerLoginMap::StartLevelTransition()
{
	UGameplayStatics::OpenLevel(this, TEXT("MainMap"));
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Login/PlayerControllerLoginMap.h"
#include "Network/ClientSocket.h"
#include "UI/Login/HUDLoginMap.h"
#include "UntilDawn/UntilDawn.h"
#include "GameInstance/UntilDawnGameInstance.h"
#include "Kismet/GamePlayStatics.h"

APlayerControllerLoginMap::APlayerControllerLoginMap()
{
	bShowMouseCursor = false;
}

void APlayerControllerLoginMap::BeginPlay()
{
	Super::BeginPlay();

	loginMapHUD = Cast<AHUDLoginMap>(GetHUD());
	if (loginMapHUD)
	{
		loginMapHUD->DSendAccountInfo.BindUFunction(this, FName("ReceiveAccountInfo"));
	}

	FInputModeUIOnly uiInputMode;
	SetInputMode(uiInputMode);
}

void APlayerControllerLoginMap::SetSocket(ClientSocket* socket)
{
	clientSocket = socket;
}

void APlayerControllerLoginMap::ReceiveAccountInfo(const FText& id, const FText& pw, const bool isLogin)
{
	clientSocket->SendAccountInfo(id, pw, isLogin);
}

void APlayerControllerLoginMap::SetSignUpMessageText(const bool isGranted)
{
	loginMapHUD->SetSignUpMessageBox(isGranted);
}

void APlayerControllerLoginMap::SetLoginMessageTextAndLoginToMap(const bool isGranted, FString playerID, const int playerNumber)
{
	loginMapHUD->SetLoginMessageBox(isGranted);
	if (isGranted)
	{
		GetWorldTimerManager().SetTimer(levelTransitionTimer, this, &APlayerControllerLoginMap::StartLevelTransition, 2.f, false);
	}
}

void APlayerControllerLoginMap::StartLevelTransition()
{
	UGameplayStatics::OpenLevel(this, TEXT("MainMap"));
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Login/HUDLoginMap.h"
#include "UI/Login/WidgetLoginScreen.h"
#include "UntilDawn/UntilDawn.h"

AHUDLoginMap::AHUDLoginMap()
{
	static ConstructorHelpers::FClassFinder<UWidgetLoginScreen> WBP_LoginScreen
	(TEXT("WidgetBlueprint'/Game/_Assets/WidgetBlueprints/LoginScreen/WBP_LoginScreen.WBP_LoginScreen_C'"));
	if (WBP_LoginScreen.Succeeded()) WidgetLoginScreenClass = WBP_LoginScreen.Class;
}

void AHUDLoginMap::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetLoginScreenClass)
	{
		LoginScreenWidget = CreateWidget<UWidgetLoginScreen>(GetOwningPlayerController(), WidgetLoginScreenClass);
		LoginScreenWidget->Init();
		LoginScreenWidget->DLoginTextBox.BindUFunction(this, FName("RequestLogin"));
		LoginScreenWidget->DSignUpTextBox.BindUFunction(this, FName("RequestSignUp"));
		LoginScreenWidget->AddToViewport();
	}
}

void AHUDLoginMap::RequestLogin(FText& id, FText& pw)
{
	DSendAccountInfo.ExecuteIfBound(id, pw, 1);
}

void AHUDLoginMap::RequestSignUp(FText& id, FText& pw)
{
	DSendAccountInfo.ExecuteIfBound(id, pw, 0);
}

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

void AHUDLoginMap::SetLoginMessageBox(const bool requestResult)
{
	FString message;
	if (requestResult) message = TEXT("로그인 성공.");
	else message = TEXT("잘못된 로그인 정보입니다.");
	LoginScreenWidget->SetLoginMessage(message, requestResult);
}

void AHUDLoginMap::SetSignUpMessageBox(const bool requestResult)
{
	FString message;
	if (requestResult) message = TEXT("회원가입 성공.");
	else message = TEXT("존재하는 아이디 입니다.");
	LoginScreenWidget->SetSignUpMessage(message, requestResult);
}

void AHUDLoginMap::RequestLogin(const FText& id, const FText& pw)
{
	DSendAccountInfo.ExecuteIfBound(id, pw, 1);
}

void AHUDLoginMap::RequestSignUp(const FText& id, const FText& pw)
{
	FString msg = CheckAccountIsValid(id.ToString());
	if (!msg.Len()) // 메시지 길이가 0이면 유효
	{
		msg = CheckAccountIsValid(pw.ToString());
		if(!msg.Len()) // 메시지 길이가 0이면 유효
			DSendAccountInfo.ExecuteIfBound(id, pw, 0);
		else
			LoginScreenWidget->SetSignUpMessage(msg, false);
	}
	else
		LoginScreenWidget->SetSignUpMessage(msg, false);
}

FString AHUDLoginMap::CheckAccountIsValid(const FString& account)
{
	if (account.Len() < 5 || account.Len() > 10)
	{
		return TEXT("5~10자로 작성해주세요.");
	}
	else
	{
		for (int i = 0; i < account.Len(); i++)
		{
			if ((account[i] >= 'A' && account[i] <= 'Z') || (account[i] >= 'a' && account[i] <= 'z') || (account[i] >= '0' && account[i] <= '9'))
				continue;
			return TEXT("영문 및 숫자로만 구성해주세요.");
		}
	}
	return TEXT("");
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Login/WidgetLoginScreen.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UWidgetLoginScreen::Init()
{
	loginMenuButton->OnClicked.AddDynamic(this, &UWidgetLoginScreen::ChangeMenuToLogin);
	signUpMenuButton->OnClicked.AddDynamic(this, &UWidgetLoginScreen::ChangeMenuToSignUp);
	loginButton->OnClicked.AddDynamic(this, &UWidgetLoginScreen::LoginToGame);
	signUpButton->OnClicked.AddDynamic(this, &UWidgetLoginScreen::SignUpToGame);
}

void UWidgetLoginScreen::SetLoginMessage(const FString& message, const bool isGreen)
{
	textBlock_Login->SetText(FText::FromString(message));
	if (isGreen)
	{
		textBlock_Login->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		textBlock_Login->SetColorAndOpacity(FLinearColor::Red);
	}
}

void UWidgetLoginScreen::SetSignUpMessage(const FString& message, const bool isGreen)
{
	textBlock_SignUp->SetText(FText::FromString(message));
	if (isGreen)
	{
		textBlock_SignUp->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		textBlock_SignUp->SetColorAndOpacity(FLinearColor::Red);
	}
}

void UWidgetLoginScreen::ChangeMenuToLogin()
{
	mainBox->SetActiveWidgetIndex(0);
}

void UWidgetLoginScreen::ChangeMenuToSignUp()
{
	mainBox->SetActiveWidgetIndex(1);
}

void UWidgetLoginScreen::LoginToGame()
{
	DLoginTextBox.ExecuteIfBound(idTextBox_Login->GetText(), pwTextBox_Login->GetText());
	idTextBox_Login->SetText(FText::FromString(FString(TEXT(""))));
	pwTextBox_Login->SetText(FText::FromString(FString(TEXT(""))));
}

void UWidgetLoginScreen::SignUpToGame()
{
	DSignUpTextBox.ExecuteIfBound(idTextBox_SignUp->GetText(), pwTextBox_SignUp->GetText());
	idTextBox_SignUp->SetText(FText::FromString(FString(TEXT(""))));
	pwTextBox_SignUp->SetText(FText::FromString(FString(TEXT(""))));
}

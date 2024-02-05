// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Login/WidgetLoginScreen.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"

void UWidgetLoginScreen::Init()
{
	LoginMenuButton->OnClicked.AddDynamic(this, &UWidgetLoginScreen::ChangeMenuToLogin);
	SignUpMenuButton->OnClicked.AddDynamic(this, &UWidgetLoginScreen::ChangeMenuToSignUp);
	LoginButton->OnClicked.AddDynamic(this, &UWidgetLoginScreen::LoginToGame);
	SignUpButton->OnClicked.AddDynamic(this, &UWidgetLoginScreen::SignUpToGame);
}

void UWidgetLoginScreen::ChangeMenuToLogin()
{
	MainBox->SetActiveWidgetIndex(0);
}

void UWidgetLoginScreen::ChangeMenuToSignUp()
{
	MainBox->SetActiveWidgetIndex(1);
}

void UWidgetLoginScreen::LoginToGame()
{
	DLoginTextBox.ExecuteIfBound(IdTextBox_Login->GetText(), PwTextBox_Login->GetText());
	IdTextBox_Login->SetText(FText::FromString(FString(TEXT(""))));
	PwTextBox_Login->SetText(FText::FromString(FString(TEXT(""))));
}

void UWidgetLoginScreen::SignUpToGame()
{
	DLoginTextBox.ExecuteIfBound(IdTextBox_SignUp->GetText(), PwTextBox_SignUp->GetText());
	IdTextBox_SignUp->SetText(FText::FromString(FString(TEXT(""))));
	PwTextBox_SignUp->SetText(FText::FromString(FString(TEXT(""))));
}

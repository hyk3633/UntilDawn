// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetLoginScreen.generated.h"

/**
 * 
 */

class UButton;
class UEditableTextBox;
class UWidgetSwitcher;

DECLARE_DELEGATE_TwoParams(DelegateTextBoxValue, FText id, FText pw);

UCLASS()
class UNTILDAWN_API UWidgetLoginScreen : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init();

	DelegateTextBoxValue DLoginTextBox, DSignUpTextBox;

protected:

	UFUNCTION()
	void ChangeMenuToLogin();

	UFUNCTION()
	void ChangeMenuToSignUp();

	UFUNCTION()
	void LoginToGame();

	UFUNCTION()
	void SignUpToGame();

private:

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MainBox;

	// 로그인 메뉴

	UPROPERTY(meta = (BindWidget))
	UButton* LoginMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoginButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IdTextBox_Login;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PwTextBox_Login;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* MessageTextBox_Login;

	// 회원가입 메뉴

	UPROPERTY(meta = (BindWidget))
	UButton* SignUpMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SignUpButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IdTextBox_SignUp;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PwTextBox_SignUp;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* MessageTextBox_SignUp;

};

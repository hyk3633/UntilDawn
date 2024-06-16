// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetLoginScreen.generated.h"

/**
 * 
 */

class UButton;
class UTextBlock;
class UEditableTextBox;
class UWidgetSwitcher;

DECLARE_DELEGATE_TwoParams(DelegateTextBoxValue, const FText id, const FText pw);

UCLASS()
class UNTILDAWN_API UWidgetLoginScreen : public UUserWidget
{
	GENERATED_BODY()

public:

	DelegateTextBoxValue DLoginTextBox, DSignUpTextBox;

	void Init();

	void SetLoginMessage(const FString&, const bool);

	void SetSignUpMessage(const FString&, const bool);

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
	UWidgetSwitcher* mainBox;

	// 로그인 메뉴

	UPROPERTY(meta = (BindWidget))
	UButton* loginMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* loginButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* idTextBox_Login;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* pwTextBox_Login;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* textBlock_Login;

	// 회원가입 메뉴

	UPROPERTY(meta = (BindWidget))
	UButton* signUpMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* signUpButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* idTextBox_SignUp;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* pwTextBox_SignUp;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* textBlock_SignUp;

};

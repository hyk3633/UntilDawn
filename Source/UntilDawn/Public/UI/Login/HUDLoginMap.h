// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDLoginMap.generated.h"

/**
 * 
 */

class UWidgetLoginScreen;

DECLARE_DELEGATE_ThreeParams(DelegateSendAccountInfo, const FText id, const FText pw, bool isLogin);

UCLASS()
class UNTILDAWN_API AHUDLoginMap : public AHUD
{
	GENERATED_BODY()

public:

	AHUDLoginMap();

	virtual void BeginPlay() override;

	void SetLoginMessageBox(const bool);

	void SetSignUpMessageBox(const bool);

	DelegateSendAccountInfo DSendAccountInfo;

protected:

	UFUNCTION()
	void RequestLogin(const FText& id, const FText& pw);

	UFUNCTION()
	void RequestSignUp(const FText& id, const FText& pw);

	FString CheckAccountIsValid(const FString& account);

private:

	UPROPERTY()
	TSubclassOf<UWidgetLoginScreen> WidgetLoginScreenClass;

	UPROPERTY()
	UWidgetLoginScreen* LoginScreenWidget;
};

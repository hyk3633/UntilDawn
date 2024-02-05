// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDLoginMap.generated.h"

/**
 * 
 */

class UWidgetLoginScreen;

DECLARE_DELEGATE_ThreeParams(DelegateSendAccountInfo, FText id, FText pw, bool isLogin);

UCLASS()
class UNTILDAWN_API AHUDLoginMap : public AHUD
{
	GENERATED_BODY()

public:

	AHUDLoginMap();

	virtual void BeginPlay() override;

	DelegateSendAccountInfo DSendAccountInfo;

protected:

	UFUNCTION()
	void RequestLogin(FText& id, FText& pw);

	UFUNCTION()
	void RequestSignUp(FText& id, FText& pw);

private:

	UPROPERTY()
	TSubclassOf<UWidgetLoginScreen> WidgetLoginScreenClass;

	UPROPERTY()
	UWidgetLoginScreen* LoginScreenWidget;
};

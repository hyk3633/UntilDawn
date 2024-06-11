// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RemotePlayerAIController.h"
#include "AbilitySystemComponent.h"

ARemotePlayerAIController::ARemotePlayerAIController()
{
	asc = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* ARemotePlayerAIController::GetAbilitySystemComponent() const
{
	return asc;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RemotePlayerAIController.h"
#include "GAS/AttributeSet/PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"

ARemotePlayerAIController::ARemotePlayerAIController()
{
	asc = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	playerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("Player Attribute Set"));
}

UAbilitySystemComponent* ARemotePlayerAIController::GetAbilitySystemComponent() const
{
	return asc;
}

void ARemotePlayerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	playerAttributeSet->SetInfinite();
	playerAttributeSet->InitStamina(10000);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Component/InstantRecoveryComponent.h"
#include "Player/PlayerCharacter.h"

UInstantRecoveryComponent::UInstantRecoveryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInstantRecoveryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UInstantRecoveryComponent::InstantHealthRecovery(TWeakObjectPtr<APlayerCharacter> playerCharacter, const float recoveryAmount)
{
	playerCharacter->RecoverHealth(recoveryAmount);
}


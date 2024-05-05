// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumable/ItemRecovery.h"
#include "Item/Component/InstantRecoveryComponent.h"
#include "Player/PlayerCharacter.h"

AItemRecovery::AItemRecovery()
{
	instantRecoveryComponent = CreateDefaultSubobject<UInstantRecoveryComponent>(TEXT("Instant Recovery Component"));
}

void AItemRecovery::InitializeRecoveryInfo(const FRecoveryItemInfo& newInfo)
{
	recoveryItemInfo = newInfo;
}

void AItemRecovery::UsingItem()
{
	// �÷��̾� ĳ���� �ִ� �ν��Ͻ��� ȸ�� ���� �ִϸ��̼� ���?
	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(GetOwner());
	instantRecoveryComponent->InstantHealthRecovery(playerCharacter, recoveryItemInfo.recoveryAmount);
}

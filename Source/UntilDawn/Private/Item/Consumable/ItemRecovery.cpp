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
	// 플레이어 캐릭터 애님 인스턴스로 회복 동작 애니메이션 재생?
	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(GetOwner());
	instantRecoveryComponent->InstantHealthRecovery(playerCharacter, recoveryItemInfo.recoveryAmount);
}

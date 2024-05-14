// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject/ItemRecovery.h"

void UItemRecovery::ParseItemConcreteInfo(TMap<FString, TSharedPtr<FJsonValue>>& concreteInfoMap)
{
	concreteInfo.Parsing(concreteInfoMap);
}

void UItemRecovery::MakeItemFunction()
{
}

void UItemRecovery::Using(TWeakObjectPtr<APlayerController> playerController, USkeletalMeshComponent* itemMesh)
{
	// 회복 애니메이션, 숫자 차감, 컨트롤러로 서버로 아이템 사용 패킷 전송
}

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
	// ȸ�� �ִϸ��̼�, ���� ����, ��Ʈ�ѷ��� ������ ������ ��� ��Ŷ ����
}

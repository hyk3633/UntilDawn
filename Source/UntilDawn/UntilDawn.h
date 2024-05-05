// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CALLINFO			(FString(__FUNCTION__) + TEXT("(")+FString::FromInt(__LINE__) + TEXT(")")) 

#define CF()				UE_LOG(LogTemp, Warning, TEXT("%s"), *CALLINFO)
#define WLOG(format)		UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *CALLINFO, *FString::Printf(format))
#define ELOG(format)		UE_LOG(LogTemp, Error, TEXT("%s : %s"), *CALLINFO, *FString::Printf(format))
#define PLOG(format, ...)	UE_LOG(LogTemp, Warning, TEXT("%s %s"), *CALLINFO, *FString::Printf(format, ##__VA_ARGS__))

#define PACKET_SIZE 4096

#define ECC_PlayerRange		ECollisionChannel::ECC_GameTraceChannel1
#define ECC_ZombieAttack	ECollisionChannel::ECC_GameTraceChannel3
#define ECC_Item			ECollisionChannel::ECC_GameTraceChannel6
#define ECC_ItemTrace		ECollisionChannel::ECC_GameTraceChannel7
#define ECC_PlayerAttack	ECollisionChannel::ECC_GameTraceChannel8
#define ECC_Player			ECollisionChannel::ECC_GameTraceChannel9
#define ECC_AimTrace		ECollisionChannel::ECC_GameTraceChannel10
#define ECC_Projectile		ECollisionChannel::ECC_GameTraceChannel11

FORCEINLINE void VectorTruncate(FVector& vector)
{
	vector.X = trunc(vector.X * 1000) * 0.001f;
	vector.Y = trunc(vector.Y * 1000) * 0.001f;
	vector.Z = trunc(vector.Z * 1000) * 0.001f;
}
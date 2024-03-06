// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CALLINFO			(FString(__FUNCTION__) + TEXT("(")+FString::FromInt(__LINE__) + TEXT(")")) 

#define CF()				UE_LOG(LogTemp, Warning, TEXT("%s"), *CALLINFO)
#define WLOG(format)		UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *CALLINFO, *FString::Printf(format))
#define ELOG(format)		UE_LOG(LogTemp, Error, TEXT("%s : %s"), *CALLINFO, *FString::Printf(format))
#define PLOG(format, ...)	UE_LOG(LogTemp, Warning, TEXT("%s %s"), *CALLINFO, *FString::Printf(format, ##__VA_ARGS__))

#define PACKET_SIZE 4096

#define ECC_PlayerRange ECollisionChannel::ECC_GameTraceChannel1

FORCEINLINE void VectorTruncate(FVector& vector)
{
	vector.X = trunc(vector.X * 1000) / 1000;
	vector.Y = trunc(vector.Y * 1000) / 1000;
	vector.Z = trunc(vector.Z * 1000) / 1000;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerAnimInst.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "UntilDawn/UntilDawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UntilDawn.h"

void UPlayerAnimInst::InitAnimInst(APlayerCharacter* character)
{
	myCharacter = character;
}

void UPlayerAnimInst::UpdateValue(float deltaTime)
{
	if (myCharacter)
	{
		targetSpeed = myCharacter->GetTargetSpeed();
		isFalling = myCharacter->GetIsFalling();
		velocityZ = myCharacter->GetVelocity().Z;
		direction = myCharacter->GetDirection();
		pitch = myCharacter->GetPitch();
		yaw = myCharacter->GetYaw();
		turnRight = myCharacter->GetTurnRight();
		turnLeft = myCharacter->GetTurnLeft();
		bAiming = myCharacter->GetAiming();
		currentWeaponType = myCharacter->GetCurrentWeaponType();

		FootIK(deltaTime);
	}
}

void UPlayerAnimInst::FootIK(float deltaTime)
{
	if (myCharacter->GetCharacterMovement()->IsFalling() == false)
	{
		ignoreActors.Emplace(myCharacter);

		TTuple<bool, float> foot_R = CapsuleDistance("ik_foot_r");
		TTuple<bool, float> foot_L = CapsuleDistance("ik_foot_l");

		if (foot_L.Get<0>() || foot_R.Get<0>())
		{
			const float selectfloat = UKismetMathLibrary::SelectFloat(foot_L.Get<1>(), foot_R.Get<1>(), foot_L.Get<1>() >= foot_R.Get<1>());
			displacement = FMath::FInterpTo(displacement, (selectfloat - 98.f) * -1.f, deltaTime, IKInterpSpeed);

			TTuple<bool, float, FVector> footTrace_R = FootLineTrace("ik_foot_r");
			TTuple<bool, float, FVector> footTrace_L = FootLineTrace("ik_foot_l");

			const float distance_R = footTrace_R.Get<1>();
			const FVector footRVector(footTrace_R.Get<2>());
			const FRotator makeRRot(UKismetMathLibrary::DegAtan2(footRVector.X, footRVector.Z) * -1.f, 0.f, UKismetMathLibrary::DegAtan2(footRVector.Y, footRVector.Z));

			RRot = FMath::RInterpTo(RRot, makeRRot, deltaTime, IKInterpSpeed);
			RIK = FMath::FInterpTo(RIK, (distance_R - 110.f) / -45.f, deltaTime, IKInterpSpeed);

			const float distance_L = footTrace_L.Get<1>();
			const FVector footLVector(footTrace_L.Get<2>());
			const FRotator makeLRot(UKismetMathLibrary::DegAtan2(footLVector.X, footLVector.Z) * -1.f, 0.f, UKismetMathLibrary::DegAtan2(footLVector.Y, footLVector.Z));

			LRot = FMath::RInterpTo(LRot, makeLRot, deltaTime, IKInterpSpeed);
			LIK = FMath::FInterpTo(LIK, (distance_L - 110.f) / -45.f, deltaTime, IKInterpSpeed);
		}
	}
	else
	{
		LRot = FMath::RInterpTo(LRot, FRotator::ZeroRotator, deltaTime, IKInterpSpeed);
		LIK = FMath::FInterpTo(LIK, 0.f, deltaTime, IKInterpSpeed);

		RRot = FMath::RInterpTo(RRot, FRotator::ZeroRotator, deltaTime, IKInterpSpeed);
		RIK = FMath::FInterpTo(RIK, 0.f, deltaTime, IKInterpSpeed);
	}
}

TTuple<bool, float> UPlayerAnimInst::CapsuleDistance(FName socketName)
{
	const FVector worldLocation{ myCharacter->GetMesh()->GetComponentLocation() };
	const FVector breakVector{ worldLocation + FVector(0.f,0.f,98.f) };

	const FVector socketLocation{ myCharacter->GetMesh()->GetSocketLocation(socketName) };

	const FVector start{ socketLocation.X,socketLocation.Y,breakVector.Z };
	const FVector end{ start - FVector(0.f,0.f,151.f) };

	FHitResult hitResult;

	UKismetSystemLibrary::LineTraceSingle(
		this,
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5),
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		hitResult,
		false);

	const bool result(hitResult.bBlockingHit);

	return MakeTuple(result, hitResult.Distance);
}

TTuple<bool, float, FVector> UPlayerAnimInst::FootLineTrace(FName socketName)
{
	const FVector socketLocation{ myCharacter->GetMesh()->GetSocketLocation(socketName) };
	const FVector rootLocation(myCharacter->GetMesh()->GetSocketLocation("Root"));

	const FVector start{ socketLocation.X,socketLocation.Y,rootLocation.Z };

	FHitResult hitResult;

	UKismetSystemLibrary::LineTraceSingle(
		this,
		start + FVector(0.f, 0.f, 105.f),
		start + FVector(0.f, 0.f, -105.f),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5),
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		hitResult,
		false);

	const bool result(hitResult.bBlockingHit);

	if (hitResult.bBlockingHit)
	{
		return MakeTuple(result, hitResult.Distance, hitResult.Normal);
	}
	else
	{
		return MakeTuple(result, 999.f, FVector::ZeroVector);
	}
}

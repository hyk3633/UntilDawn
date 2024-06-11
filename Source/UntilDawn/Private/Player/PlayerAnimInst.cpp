// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerAnimInst.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "UntilDawn/UntilDawn.h"

void UPlayerAnimInst::InitAnimInst(APlayerCharacter* character)
{
	myCharacter = character;
}

void UPlayerAnimInst::UpdateValue()
{
	if (myCharacter)
	{
		speed = myCharacter->GetSpeed();
		isFalling = myCharacter->GetIsFalling();
		velocityZ = myCharacter->GetVelocity().Z;
		direction = myCharacter->GetDirection();
		pitch = myCharacter->GetPitch();
		yaw = myCharacter->GetYaw();
		turnRight = myCharacter->GetTurnRight();
		turnLeft = myCharacter->GetTurnLeft();
		bAiming = myCharacter->GetAiming();
		currentWeaponType = myCharacter->GetCurrentWeaponType();
	}
}